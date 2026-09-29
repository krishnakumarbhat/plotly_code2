% Read bin files and store the data in .mat files so that it can be easily read in Python using a
% third-party package. This enables us to directly compare the data output from the MatLab bin
% reader and pybin. The output of pybin is expected to match, exactly, the data from the  .mat file.
%
% Since the MatLab bin reader can output the data in two different formats we save two .mat files
% for each bin file.

% Input data
base_dir = pwd;
extract_dir = fullfile(pwd, 'tmp');
bin_paths = unzip('bin.zip', extract_dir);

% Output data will be stored here
mat_dir = fullfile(extract_dir, 'mat');
mat_signal_dir = fullfile(mat_dir, 'signal_indexing');
mat_sample_dir = fullfile(mat_dir, 'sample_indexing');
mkdir(mat_signal_dir)
mkdir(mat_sample_dir)

% Change directory to where the bin reader is located
cd('..\..\Matlab\');

for i = 1:length(bin_paths)
    % Read the bin file in indexed by signal mode
    bin_path = bin_paths{i};
    data = read_log_data(bin_path, 0);

    % Save the ouput to a mat file
    [~, f_name, f_ext] = fileparts(bin_path);
    mat_file = fullfile(mat_signal_dir, strcat(f_name, '.mat'));
    save(mat_file, 'data');

    % Read the bin file in indexed by sample mode
    data = read_log_data(bin_path, 1);

    % Save the ouput to a mat file
    mat_file = fullfile(mat_sample_dir, strcat(f_name, '.mat'));
    save(mat_file, 'data');
end

% Zip mat files for use in Python tests
cd(base_dir)
zip('mat', '*', mat_dir)

% Delete temporary files
rmdir(extract_dir, 's')
