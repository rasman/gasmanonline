% matlab_example.m — GasMan simulation from MATLAB
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
% Usage (MATLAB command window or script):
%   matlab_example           % uses examples/example_1.xml as default input
%   matlab_example           % edit INPUT_FILE / OUTPUT_FILE below to change
%
% Usage (from the OS shell):
%   matlab -batch "cd examples; matlab_example"

% ---------------------------------------------------------------------------
% Configuration — edit these to change inputs / outputs
% ---------------------------------------------------------------------------
INPUT_FILE  = fullfile(fileparts(mfilename('fullpath')), 'example_1.xml');
OUTPUT_FILE = fullfile(fileparts(mfilename('fullpath')), 'output_matlab.csv');
START_SEC   = 0;
END_SEC     = 300;
EVERY_SEC   = 10;

% ---------------------------------------------------------------------------
% Locate the compiled/ directory (one level above examples/)
% ---------------------------------------------------------------------------
examples_dir  = fileparts(mfilename('fullpath'));
repo_root     = fileparts(examples_dir);
compiled_dir  = fullfile(repo_root, 'compiled');

% ============================================================================
% Approach A — DLL via loadlibrary / calllib
% ============================================================================
fprintf('--- Approach A: DLL ---\n');

% Determine platform-specific library filename.
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

    % loadlibrary parses gasmanAPI.h to learn the function signatures.
    % On Linux/macOS a thunk file is compiled on first use; subsequent calls
    % use the cached thunk automatically.
    try
        loadlibrary(lib_file, header_file, 'alias', 'gasmanAPI');
        fprintf('Loaded: %s\n', lib_file);
    catch ME
        warning('loadlibrary failed: %s', ME.message);
    end

    if libisloaded('gasmanAPI')
        % ── Build a scenario in the GasMan native JSON format ────────────────
        scenario = gasman_build_scenario();
        json_str = jsonencode(scenario);   % requires MATLAB R2016b+

        % ── Call GasManJsonToCsv ─────────────────────────────────────────────
        % Arguments: json_str (char*), len (int), start, end, every (int).
        % Returns:   const char* — MATLAB represents this as a libpointer.
        result_ptr = calllib('gasmanAPI', 'GasManJsonToCsv', ...
            json_str, int32(numel(json_str)), ...
            int32(START_SEC), int32(END_SEC), int32(EVERY_SEC));

        % ── Check error code ─────────────────────────────────────────────────
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

        % ── Convert libpointer → char array → parse CSV ──────────────────────
        if isa(result_ptr, 'libpointer')
            csv_text = char(result_ptr.Value);
        else
            csv_text = char(result_ptr);
        end

        result_A = gasman_parse_csv(csv_text);
        fprintf('DLL simulation complete: %d rows\n', height(result_A));
        disp(head(result_A, 3));

        % ── Write output ─────────────────────────────────────────────────────
        writetable(result_A, OUTPUT_FILE);
        fprintf('Written: %s\n', OUTPUT_FILE);

        % Always unload to free the DLL handle.
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
        runner, INPUT_FILE, START_SEC, END_SEC, EVERY_SEC, tmp_csv);

    [status, out] = system(cmd);

    if status ~= 0 || ~isfile(tmp_csv)
        error('gasman_run failed (exit %d):\n%s', status, out);
    end

    result_B = readtable(tmp_csv);
    fprintf('CLI simulation complete: %d rows\n', height(result_B));
    disp(head(result_B, 3));

    delete(tmp_csv);
end


% ============================================================================
% Helper functions
% ============================================================================

function scenario = gasman_build_scenario()
% Build a minimal GasMan scenario in the native JSON format.
% Edit the values here or replace this function with a call to your own
% scenario-building logic (e.g. loading from an Excel workbook with
% readmatrix / readtable).

    scenario = struct();
    scenario.description = '';

    % Patient
    patient.weight.value = 70;
    patient.weight.unit  = 'kilograms';
    patient.volumes.vrg  = 6.0;
    patient.volumes.fat  = 14.5;
    patient.volumes.ven  = 1.0;
    patient.volumes.alv  = 2.5;
    patient.volumes.mus  = 33.0;
    patient.flows.vrg    = 0.76;
    patient.flows.fat    = 0.06;
    patient.flows.mus    = 0.18;

    % Agent (Sevoflurane defaults)
    agent.name        = 'Sevoflurane';
    agent.lambdaBlood = 0.65;
    agent.lambdaVrg   = 1.1;
    agent.lambdaFat   = 34;
    agent.lambdaMus   = 2.4;
    agent.volatility  = 183;
    agent.bottleSize  = 250;
    agent.bottleCost  = 75;

    scenario.params.patient = patient;
    scenario.params.agents  = {agent};   % cell array → JSON array

    % Settings (one time-step)
    setting.time    = '00:00:00';
    setting.va      = 4;
    setting.fgf     = 8;
    setting.co      = 5;
    setting.circuit = 'Semi';
    setting.agent_settings = {struct('name', 'Sevoflurane', 'del', 3.7, 'inject', 0)};

    scenario.settings = {setting};       % cell array → JSON array
end


function T = gasman_parse_csv(csv_text)
% Parse the CSV string returned by GasManJsonToCsv into a MATLAB table.
%
% Writes to a tempfile because textscan / readtable work more reliably
% on files than on in-memory strings in all supported MATLAB versions.

    tmp = [tempname, '.csv'];
    fid = fopen(tmp, 'w');
    fwrite(fid, csv_text, 'char');
    fclose(fid);
    T = readtable(tmp);
    delete(tmp);
end
