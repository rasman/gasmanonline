function octave_example(input_file, output_file)
% octave_example — GasMan simulation from GNU Octave via gasman_run CLI
%
% gasman_run handles all input parsing internally, so no XML/JSON/CSV/XLSX
% parsers are needed here.  All four formats are supported transparently.
%
% Why no DLL approach?
%   MATLAB's loadlibrary/calllib are not available in Octave.  Calling a
%   shared library from Octave requires a compiled .oct extension — outside
%   the scope of this example.  For DLL access from a scripting language,
%   use python_example.py or r_example.R instead.
%
% Requires:
%   GNU Octave 6 or later
%   gasman_run binary in the same directory as this script, in the current
%   working directory, or in <repo>/compiled/
%
% Usage (Octave session):
%   octave_example                                  % default: example_1.xml
%   octave_example('simulation_template.csv')
%   octave_example('test.json', 'my_output.csv')
%
% Usage (OS shell):
%   octave --no-gui --eval "octave_example('simulation_template.csv')"

  if nargin < 1 || isempty(input_file)
    input_file = fullfile(fileparts(mfilename('fullpath')), 'example_1.xml');
  end
  if nargin < 2 || isempty(output_file)
    output_file = fullfile(fileparts(mfilename('fullpath')), 'output_octave.csv');
  end

  START_SEC = 0;
  END_SEC   = 300;
  EVERY_SEC = 10;

  % ── Locate gasman_run ───────────────────────────────────────────────────────
  examples_dir = fileparts(mfilename('fullpath'));
  repo_root    = fileparts(examples_dir);

  if ispc()
    runner_name = 'gasman_run.exe';
  else
    runner_name = 'gasman_run';
  end

  % Search: same directory as this script → cwd → repo compiled/
  candidates = {
    fullfile(examples_dir,          runner_name),
    fullfile(pwd(),                 runner_name),
    fullfile(repo_root, 'compiled', runner_name),
  };

  runner = '';
  for i = 1 : numel(candidates)
    if exist(candidates{i}, 'file') == 2
      runner = candidates{i};
      break;
    end
  end

  if isempty(runner)
    error('gasman_run not found. Looked in:\n  %s\nBuild the project first.', ...
          strjoin(candidates, '\n  '));
  end

  % ── Run simulation ─────────────────────────────────────────────────────────
  tmp_csv = [tempname(), '.csv'];
  cmd = sprintf('"%s" "%s" --start %d --end %d --every %d --output "%s"', ...
      runner, input_file, START_SEC, END_SEC, EVERY_SEC, tmp_csv);

  [status, out] = system(cmd);

  if status ~= 0
    if exist(tmp_csv, 'file') == 2, delete(tmp_csv); end
    error('gasman_run failed (exit %d):\n%s', status, out);
  end
  if exist(tmp_csv, 'file') ~= 2
    error('gasman_run produced no output file.\n%s', out);
  end

  % ── Display results ─────────────────────────────────────────────────────────
  % Read all lines so we can count rows and show a preview without needing
  % readtable (which requires Octave 7+ for reliable CSV support).
  fid   = fopen(tmp_csv, 'r');
  lines = {};
  while ~feof(fid)
    ln = fgetl(fid);
    if ischar(ln)
      lines{end+1} = ln; %#ok<AGROW>
    end
  end
  fclose(fid);

  ndata = max(0, numel(lines) - 1);   % subtract header line
  fprintf('Simulation complete — %d data rows\n', ndata);

  % Print header + first 3 data rows
  fprintf('Preview:\n');
  for i = 1 : min(4, numel(lines))
    fprintf('  %s\n', lines{i});
  end

  % ── Save output ─────────────────────────────────────────────────────────────
  copyfile(tmp_csv, output_file);
  delete(tmp_csv);
  fprintf('Written: %s\n', output_file);

end
