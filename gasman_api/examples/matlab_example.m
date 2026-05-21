function matlab_example(input_file, output_file)
% matlab_example — GasMan simulation from MATLAB
%
% Demonstrates two approaches:
%
%   Approach A  DLL via loadlibrary / calllib
%     Loads gasmanAPI directly and calls GasManJsonToCsv, GasManLastError,
%     and GasManErrorString through MATLAB's built-in FFI.
%     Requires: MATLAB R2018a or later.
%
%   Approach B  CLI via system()
%     Calls the gasman_run binary.  No DLL loading, no header parsing.
%     Works with any MATLAB version that has system().
%
% Usage (MATLAB command window):
%   matlab_example                                  % default: example_1.xml
%   matlab_example('simulation_template.csv')
%   matlab_example('test.json', 'my_output.csv')
%
% Usage (from the OS shell):
%   matlab -batch "cd('examples'); matlab_example('simulation_template.csv')"
%
% Supported input formats: .xml, .json, .csv, .xlsx

if nargin < 1 || isempty(input_file)
    input_file = fullfile(fileparts(mfilename('fullpath')), 'example_1.xml');
end
if nargin < 2 || isempty(output_file)
    output_file = fullfile(fileparts(mfilename('fullpath')), 'output_matlab.csv');
end

START_SEC = 0;
EVERY_SEC = 10;

% ---------------------------------------------------------------------------
% Locate the compiled/ directory (one level above examples/)
% ---------------------------------------------------------------------------
examples_dir = fileparts(mfilename('fullpath'));
repo_root    = fileparts(examples_dir);
compiled_dir = fullfile(repo_root, 'compiled');

% ── Parse scenario once to determine simulation end time ────────────────────
% Both Approach A (DLL) and Approach B (CLI) reuse this result.
END_SEC  = 300;
scenario = struct();
try
    scenario = gasman_parse_input_file(input_file);
    END_SEC  = max(END_SEC, gasman_last_sec(scenario));
catch ME
    warning('gasman:parse', 'Pre-parse for end-time detection failed: %s', ME.message);
end

% ============================================================================
% Approach A — DLL via loadlibrary / calllib
% ============================================================================
fprintf('--- Approach A: DLL ---\n');

if ispc
    lib_file = fullfile(compiled_dir, 'gasmanAPI.dll');
elseif ismac
    lib_file = fullfile(compiled_dir, 'gasmanAPI.dylib');
else
    lib_file = fullfile(compiled_dir, 'gasmanAPI.so');
end

header_file = fullfile(repo_root, 'gasmanAPI', 'gasmanAPI.h');

if ~isfile(lib_file)
    warning('Library not found: %s\nBuild the project first (cmake --build build --config Release).', lib_file);
else
    % Load the library (unload first if already loaded from a previous run).
    if libisloaded('gasmanAPI')
        unloadlibrary('gasmanAPI');
    end

    try
        loadlibrary(lib_file, header_file, 'alias', 'gasmanAPI');
        fprintf('Loaded: %s\n', lib_file);
    catch ME
        warning('loadlibrary failed: %s', ME.message);
    end

    if libisloaded('gasmanAPI')
        % ── Use the pre-parsed scenario ───────────────────────────────────────
        json_str = jsonencode(scenario);   % requires MATLAB R2016b+

        % ── Call GasManJsonToCsv ──────────────────────────────────────────────
        result_ptr = calllib('gasmanAPI', 'GasManJsonToCsv', ...
            json_str, int32(numel(json_str)), ...
            int32(START_SEC), int32(END_SEC), int32(EVERY_SEC));

        % ── Check error code ──────────────────────────────────────────────────
        err_code = calllib('gasmanAPI', 'GasManLastError');

        if err_code ~= 0
            err_ptr = calllib('gasmanAPI', 'GasManErrorString', int32(err_code));
            if isa(err_ptr, 'libpointer')
                err_msg = char(err_ptr.Value);
            else
                err_msg = char(err_ptr);
            end
            error('GasManJsonToCsv failed [%d: %s]', err_code, err_msg);
        end

        % ── Convert libpointer → char array → parse CSV ───────────────────────
        if isa(result_ptr, 'libpointer')
            csv_text = char(result_ptr.Value);
        else
            csv_text = char(result_ptr);
        end

        result_A = gasman_csv_to_table(csv_text);
        fprintf('DLL simulation complete: %d rows\n', height(result_A));
        disp(head(result_A, 3));

        % ── Write output ──────────────────────────────────────────────────────
        writetable(result_A, output_file);
        fprintf('Written: %s\n', output_file);

        unloadlibrary('gasmanAPI');
    end
end

% ============================================================================
% Approach B — CLI via system(gasman_run)
% ============================================================================
fprintf('\n--- Approach B: CLI ---\n');

if ispc
    runner = fullfile(compiled_dir, 'gasman_run.exe');
else
    runner = fullfile(compiled_dir, 'gasman_run');
end

if ~isfile(runner)
    warning('gasman_run not found: %s', runner);
else
    tmp_csv = [tempname, '.csv'];
    cmd = sprintf('"%s" "%s" --start %d --end %d --every %d --output "%s"', ...
        runner, input_file, START_SEC, END_SEC, EVERY_SEC, tmp_csv);

    [status, out] = system(cmd);

    if status ~= 0 || ~isfile(tmp_csv)
        error('gasman_run failed (exit %d):\n%s', status, out);
    end

    result_B = readtable(tmp_csv);
    fprintf('CLI simulation complete: %d rows\n', height(result_B));
    disp(head(result_B, 3));

    delete(tmp_csv);
end

end  % matlab_example


% ============================================================================
% Input parsers — dispatch on file extension
% ============================================================================

function scenario = gasman_parse_input_file(file_path)
% Parse an input file (.xml, .json, .csv, or .xlsx) into a GasMan JSON struct.
    [~, ~, ext] = fileparts(file_path);
    switch lower(ext)
        case '.xml',  scenario = gasman_parse_xml(file_path);
        case '.json', scenario = gasman_parse_json(file_path);
        case '.csv',  scenario = gasman_parse_csv_input(file_path);
        case '.xlsx', scenario = gasman_parse_xlsx(file_path);
        otherwise
            error('Unsupported file type: ''%s''  (use .xml, .json, .csv, or .xlsx)', ext);
    end
end


% ── XML ──────────────────────────────────────────────────────────────────────

function scenario = gasman_parse_xml(file_path)
% Parse a GasMan XML scenario into a tagName-keyed struct the DLL can consume.
    dom      = xmlread(file_path);
    scenario = xml_node_to_struct(dom.getDocumentElement());
end

function s = xml_node_to_struct(node)
% Recursively convert a DOM node to a tagName-keyed struct.
    s.tagName = char(node.getNodeName());

    % Attributes
    attrs = node.getAttributes();
    for i = 0 : attrs.getLength() - 1
        attr = attrs.item(i);
        key  = matlab.lang.makeValidName(char(attr.getName()));
        val  = char(attr.getValue());
        num  = str2double(val);
        if ~isnan(num)
            s.(key) = num;
        else
            s.(key) = val;
        end
    end

    % Text content and child elements
    children   = node.getChildNodes();
    text_parts = {};
    for i = 0 : children.getLength() - 1
        child = children.item(i);
        switch double(child.getNodeType())
            case 3  % TEXT_NODE
                t = strtrim(char(child.getNodeValue()));
                if ~isempty(t)
                    text_parts{end+1} = t; %#ok<AGROW>
                end
            case 1  % ELEMENT_NODE
                tag = matlab.lang.makeValidName(char(child.getNodeName()));
                cs  = xml_node_to_struct(child);
                if isfield(s, tag)
                    if ~iscell(s.(tag))
                        s.(tag) = {s.(tag)};
                    end
                    s.(tag){end+1} = cs;
                else
                    s.(tag) = cs;
                end
        end
    end
    if ~isempty(text_parts)
        s.text = strjoin(text_parts, '');
    end
end


% ── JSON ─────────────────────────────────────────────────────────────────────

function scenario = gasman_parse_json(file_path)
% Load a GasMan JSON file (native or tagName format).
    scenario = jsondecode(fileread(file_path));
    if isfield(scenario, 'imageData')
        scenario = rmfield(scenario, 'imageData');
    end
end


% ── CSV (sections-based input) ───────────────────────────────────────────────

function scenario = gasman_parse_csv_input(file_path)
% Parse a GasMan sections-based CSV file into GasMan native JSON struct.
%
% Recognised sections (case-insensitive; lines starting with # are comments):
%   [patient]   weight_kg
%   [volumes]   vrg, fat, ven, alv, mus
%   [flows]     vrg, fat, mus
%   [agent]     name, lambdaMus, lambdaBlood, ...  (repeat for each agent)
%   [settings]  header row then data rows

    fid = fopen(file_path, 'r');
    if fid < 0
        error('Cannot open file: %s', file_path);
    end

    section       = '';
    patient       = struct();
    volumes       = struct();
    flows         = struct();
    agents        = {};
    cur_agent     = [];
    hdr           = {};
    settings_rows = {};

    while ~feof(fid)
        line = strtrim(fgetl(fid));
        if ~ischar(line) || isempty(line) || strncmp(line, '#', 1)
            continue
        end

        % Section header?
        if strncmp(line, '[', 1)
            tok = regexp(line, '^\[([^\]]+)\]', 'tokens', 'once');
            if ~isempty(tok)
                new_sec = lower(strtrim(tok{1}));
                if strcmp(new_sec, 'agent')
                    if ~isempty(cur_agent)
                        agents{end+1} = cur_agent; %#ok<AGROW>
                    end
                    cur_agent = struct();
                elseif strcmp(new_sec, 'settings')
                    hdr = {};
                end
                section = new_sec;
            end
            continue
        end

        parts = strtrim(strsplit(line, ',', 'CollapseDelimiters', false));

        switch section
            case 'patient'
                if numel(parts) >= 2
                    patient.(matlab.lang.makeValidName(parts{1})) = parts{2};
                end
            case 'volumes'
                if numel(parts) >= 2
                    volumes.(matlab.lang.makeValidName(parts{1})) = str2double(parts{2});
                end
            case 'flows'
                if numel(parts) >= 2
                    flows.(matlab.lang.makeValidName(parts{1})) = str2double(parts{2});
                end
            case 'agent'
                if ~isempty(cur_agent) && numel(parts) >= 2
                    cur_agent.(matlab.lang.makeValidName(parts{1})) = parts{2};
                end
            case 'settings'
                if isempty(hdr)
                    hdr = lower(parts);
                else
                    settings_rows{end+1} = parts; %#ok<AGROW>
                end
        end
    end
    fclose(fid);

    % Flush last agent block
    if ~isempty(cur_agent)
        agents{end+1} = cur_agent;
    end

    if isempty(agents)
        error('No [agent] block found in CSV: %s', file_path);
    end
    if isempty(settings_rows)
        error('No [settings] data found in CSV: %s', file_path);
    end

    % ── Build agent objects ───────────────────────────────────────────────────
    num_agents     = numel(agents);
    numeric_fields = {'lambdaMus','lambdaBlood','lambdaFat', ...
                      'volatility','bottleSize','lambdaVrg','bottleCost'};
    agent_objects  = cell(1, num_agents);
    for i = 1 : num_agents
        ag  = agents{i};
        obj = struct('name', '');
        if isfield(ag, 'name'), obj.name = ag.name; end
        for fi = 1 : numel(numeric_fields)
            fn  = numeric_fields{fi};
            fnl = lower(fn);
            if isfield(ag, fn)
                obj.(fn) = str2double(ag.(fn));
            elseif isfield(ag, fnl)
                obj.(fn) = str2double(ag.(fnl));
            end
        end
        agent_objects{i} = obj;
    end

    % ── Build setting objects ─────────────────────────────────────────────────
    setting_objects = cell(1, numel(settings_rows));
    for ri = 1 : numel(settings_rows)
        row     = settings_rows{ri};
        row_map = struct();
        for ci = 1 : min(numel(hdr), numel(row))
            k = matlab.lang.makeValidName(hdr{ci});
            if ~isempty(k)
                row_map.(k) = row{ci};
            end
        end

        % Per-agent del/inject columns (del1/inject1 for multi-agent, del/inject for single)
        ag_settings = cell(1, num_agents);
        for ai = 1 : num_agents
            if num_agents > 1
                dk = matlab.lang.makeValidName(sprintf('del%d',    ai));
                jk = matlab.lang.makeValidName(sprintf('inject%d', ai));
            else
                dk = 'del';
                jk = 'inject';
            end
            dv = 0;  jv = 0;
            if isfield(row_map, dk), dv = str2double(row_map.(dk)); end
            if isfield(row_map, jk), jv = str2double(row_map.(jk)); end
            ag_settings{ai} = struct('name', agent_objects{ai}.name, ...
                                     'del', dv, 'inject', jv);
        end

        ts = '00:00:00'; if isfield(row_map,'time'),    ts = row_map.time;              end
        va = 4.0;        if isfield(row_map,'va'),      va = str2double(row_map.va);    end
        fg = 5.0;        if isfield(row_map,'fgf'),     fg = str2double(row_map.fgf);   end
        co = 5.0;        if isfield(row_map,'co'),      co = str2double(row_map.co);    end
        ck = 'Semi';     if isfield(row_map,'circuit'), ck = row_map.circuit;           end

        setting_objects{ri} = struct('time', ts, 'va', va, 'fgf', fg, 'co', co, ...
            'circuit', ck, 'agent_settings', {ag_settings});
    end

    % ── Assemble native JSON struct ───────────────────────────────────────────
    wkg = 70.0;
    if isfield(patient, 'weight_kg')
        wkg = str2double(patient.weight_kg);
    end

    scenario                              = struct('description', '');
    scenario.params.patient.weight.value  = wkg;
    scenario.params.patient.weight.unit   = 'kilograms';
    scenario.params.patient.volumes       = volumes;
    scenario.params.patient.flows         = flows;
    scenario.params.agents                = agent_objects;
    scenario.settings                     = setting_objects;
end


% ── XLSX ─────────────────────────────────────────────────────────────────────

function scenario = gasman_parse_xlsx(file_path)
% Parse a GasMan Excel workbook (gasman_template.xlsx layout).
%
% Layout expected in the first sheet:
%   Row containing "Patient - Weight"  → weight + compartment data
%   Row containing "Agents"            → column headers + agent rows
%   Row containing "Settings"          → column headers + setting rows

    T   = readtable(file_path, 'ReadVariableNames', false, 'ReadRowNames', false);
    raw = table2cell(T);

    p = find_raw_row(raw, 'patient - weight');
    a = find_raw_row(raw, 'agents');
    s = find_raw_row(raw, 'settings');

    if p < 0 || a < 0 || s < 0
        error('Excel file missing expected sections (Patient - Weight / Agents / Settings).');
    end

    % ── Patient ───────────────────────────────────────────────────────────────
    patient_s.weight.value = double(raw{p+1, 3});
    patient_s.weight.unit  = 'kilograms';
    patient_s.volumes.vrg  = double(raw{p+3, 2});
    patient_s.volumes.fat  = double(raw{p+4, 2});
    patient_s.volumes.ven  = double(raw{p+5, 2});
    patient_s.volumes.alv  = double(raw{p+6, 2});
    patient_s.volumes.mus  = double(raw{p+7, 2});
    patient_s.flows.vrg    = double(raw{p+9,  2});
    patient_s.flows.fat    = double(raw{p+10, 2});
    patient_s.flows.mus    = double(raw{p+11, 2});

    % ── Agents ────────────────────────────────────────────────────────────────
    % Row a+1 is the column header; rows a+2 … s-1 are agent data rows.
    agent_hdr = raw_row_to_strings(raw(a+1, :));
    agents    = {};
    for r = a+2 : s-1
        row = raw(r, :);
        if all(cellfun(@(x) is_blank(x), row)), continue; end
        ag = struct();
        for ci = 1 : min(numel(agent_hdr), numel(row))
            if ~is_blank(row{ci})
                key = matlab.lang.makeValidName(lower(agent_hdr{ci}));
                val = row{ci};
                if isnumeric(val)
                    ag.(key) = val;
                else
                    ag.(key) = char(string(val));
                end
            end
        end
        agents{end+1} = ag; %#ok<AGROW>
    end

    if isempty(agents)
        error('No agent rows found in Excel file.');
    end
    agent_name = '';
    if isfield(agents{1}, 'name'), agent_name = agents{1}.name; end

    % ── Settings ──────────────────────────────────────────────────────────────
    % Row s+1 is the column header; rows s+2 … end are setting rows.
    set_hdr  = raw_row_to_strings(raw(s+1, :));
    set_hdr  = strrep(lower(set_hdr), ' ', '_');
    settings = {};
    for r = s+2 : size(raw, 1)
        row = raw(r, :);
        if all(cellfun(@(x) is_blank(x), row)), continue; end

        row_map = struct();
        for ci = 1 : min(numel(set_hdr), numel(row))
            k = matlab.lang.makeValidName(set_hdr{ci});
            if ~isempty(k) && ~is_blank(row{ci})
                row_map.(k) = row{ci};
            end
        end

        % Time conversion (Excel may store as duration/datetime fraction or string)
        ts = '00:00:00';
        if isfield(row_map, 'time')
            tv = row_map.time;
            if isduration(tv)
                ts = char(string(tv, 'hh:mm:ss'));
            elseif isdatetime(tv)
                ts = datestr(tv, 'HH:MM:SS');
            elseif isnumeric(tv)
                total_sec = round(tv * 86400);
                ts = sprintf('%02d:%02d:%02d', ...
                    floor(total_sec / 3600), ...
                    floor(mod(total_sec, 3600) / 60), ...
                    mod(total_sec, 60));
            else
                ts = char(string(tv));
                % Normalise HH:MM → HH:MM:SS; engine requires 3 components
                if sum(ts == ':') == 1
                    ts = [ts, ':00'];
                end
            end
        end

        va = 4.0; if isfield(row_map,'va'),      va = double(row_map.va);              end
        fg = 5.0; if isfield(row_map,'fgf'),     fg = double(row_map.fgf);             end
        co = 5.0; if isfield(row_map,'co'),       co = double(row_map.co);              end
        ck = 'Semi'; if isfield(row_map,'circuit'), ck = char(string(row_map.circuit)); end
        dv = 0.0; if isfield(row_map,'del'),     dv = double(row_map.del);             end
        jv = 0.0; if isfield(row_map,'inject'),  jv = double(row_map.inject);          end

        settings{end+1} = struct('time', ts, 'va', va, 'fgf', fg, 'co', co, ...
            'circuit', ck, ...
            'agent_settings', {{struct('name', agent_name, 'del', dv, 'inject', jv)}}); %#ok<AGROW>
    end

    scenario                    = struct('description', '');
    scenario.params.patient     = patient_s;
    scenario.params.agents      = agents;
    scenario.settings           = settings;
end


% ── XLSX helper: find a keyword row ──────────────────────────────────────────

function idx = find_raw_row(raw, keyword)
% Return the 1-based row index of the first cell containing keyword (case-insensitive).
    idx   = -1;
    nrows = size(raw, 1);
    ncols = size(raw, 2);
    for r = 1 : nrows
        for c = 1 : ncols
            v = raw{r, c};
            if ischar(v) && contains(lower(v), lower(keyword))
                idx = r;
                return
            end
        end
    end
end

function strs = raw_row_to_strings(row)
% Convert a cell-array row to a cell array of char strings; blanks become ''.
    strs = cell(1, numel(row));
    for i = 1 : numel(row)
        v = row{i};
        if is_blank(v)
            strs{i} = '';
        elseif isnumeric(v)
            strs{i} = num2str(v);
        else
            strs{i} = char(string(v));
        end
    end
end

function tf = is_blank(v)
% True if v is missing/NaN/empty string.
    if iscell(v), v = v{1}; end
    if isnumeric(v), tf = isnan(v); return; end
    if ischar(v),    tf = isempty(strtrim(v)); return; end
    try
        tf = ismissing(v);
    catch
        tf = false;
    end
end


% ============================================================================
% Helper: parse CSV text output from GasManJsonToCsv into a MATLAB table
% ============================================================================

function sec = gasman_last_sec(scenario)
% Return the last setting's time in seconds (minimum 0).
    sec = 0;
    if ~isstruct(scenario) || ~isfield(scenario, 'settings'), return; end
    S = scenario.settings;
    if iscell(S),      last = S{end};
    elseif isstruct(S), last = S(end);
    else,              return; end
    if isfield(last, 'time')
        sec = gasman_time_to_sec(last.time);
    end
end

function sec = gasman_time_to_sec(ts)
% Parse HH:MM:SS (or HH:MM) time string to seconds.
    parts = strsplit(char(ts), ':');
    if numel(parts) >= 3
        sec = str2double(parts{1})*3600 + str2double(parts{2})*60 + str2double(parts{3});
    elseif numel(parts) == 2
        sec = str2double(parts{1})*3600 + str2double(parts{2})*60;
    else
        sec = 0;
    end
    if isnan(sec), sec = 0; end
end


function T = gasman_csv_to_table(csv_text)
% Parse the CSV string returned by GasManJsonToCsv into a MATLAB table.
%
% Writes to a temp file because readtable works more reliably on files
% than on in-memory strings across all supported MATLAB versions.
    tmp = [tempname, '.csv'];
    fid = fopen(tmp, 'w');
    fwrite(fid, csv_text, 'char');
    fclose(fid);
    T = readtable(tmp);
    delete(tmp);
end
