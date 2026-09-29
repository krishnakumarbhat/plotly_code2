function [data, warnmsg] = read_log_data(filename,aos_flag,silent)

persistent last_tic;
warnmsg = '';
if isempty(last_tic)
    tic; 
    last_tic = toc;
end
if toc - last_tic > 5.0
    tic;
end
if nargin < 2
    aos_flag = 0;
end
if nargin < 3
    silent = 1;
end

enable_size = false;
if(~exist(filename, 'file'))
    filename = [filename '32'];
end
fid = fopen(filename,'rb');
if(strfind(filename, '.bin32') >0)
    file_precision = 'float32';
    file_precision_byte_size = 4;
else
    file_precision = 'float64';
     file_precision_byte_size = 8;
end
if fid==-1
    warnmsg = ['Could not open file ''' filename '''!'];
    warning(warnmsg);
    if aos_flag
        data = [];
    else
        data.scanIndex = [];
    end
    return;
end    

dir_data = dir(filename);
if dir_data.bytes == 0
    data = [];
    warnmsg = ['Empty file ''' filename ''''];
    warning(warnmsg);
    fclose(fid);
    return;
end

MAX_LEVELS = 6;

% process header info
num_defs = fread(fid,1,file_precision);%######file_precision#####
defs = cell(num_defs,1);
valid = zeros(num_defs,1);
repeat_count = zeros(num_defs,1);
repeat_count_all = zeros(num_defs,MAX_LEVELS);
repeat_skip = zeros(num_defs,MAX_LEVELS);
repeat_first = zeros(num_defs,1);
repeat_level = zeros(num_defs,1);
current_repeat_start = zeros(1,MAX_LEVELS);
current_repeat_counter = zeros(1,MAX_LEVELS);
current_repeat_count = zeros(1,MAX_LEVELS);
current_repeat_skip = zeros(1,MAX_LEVELS);
current_level = 0;
current_start = 0;
for i = 1:num_defs
    temp = fread(fid,[1 256],'char');
    defs{i} = char(temp(1:find(temp==0,1)-1));
    if length(defs{i})>6 && strcmp(defs{i}(1:6),'REPEAT')
        current_level = current_level + 1;
        current_repeat_start(current_level) = i;
        current_repeat_counter(current_level) = 0;
        current_repeat_skip(current_level) = 0;
        current_repeat_count(current_level) = str2double(defs{i}(7:end));
    elseif length(defs{i})>=10 && strcmp(defs{i}(1:10),'END_REPEAT')
        true_repeat_count = prod(current_repeat_count(1:current_level));
        repeat_len = current_repeat_counter(current_level);
        repeat_skip(current_repeat_start(current_level)+1:i-1,current_level) = current_repeat_skip(current_level);
        current_start = current_start + current_repeat_skip(current_level)*(current_repeat_count(current_level)-1);
        if current_level>1
            current_repeat_skip(current_level-1) = current_repeat_skip(current_level-1)+current_repeat_count(current_level)*current_repeat_counter(current_level);
            current_repeat_counter(current_level-1) = current_repeat_counter(current_level-1)+current_repeat_count(current_level)*current_repeat_counter(current_level);
        end
        current_level = current_level - 1;
    elseif ~enable_size
        valid(i) = 1;
        current_start = current_start + 1;
        if current_level==0
            repeat_count(i) = 1;
        else
            repeat_count(i) = prod(current_repeat_count(1:current_level));
            repeat_count_all(i,1:current_level) = current_repeat_count(1:current_level);
            current_repeat_counter(current_level) = current_repeat_counter(current_level) + 1;
            current_repeat_skip(current_level) = current_repeat_skip(current_level) + 1;
        end
        repeat_first(i) = current_start;
        repeat_level(i) = current_level;
    else
        valid(i) = 1;
        types{i} = file_precision;
        sz = file_precision_byte_size;
        current_level = current_level + 1;
        current_repeat_start(current_level) = i;
        current_repeat_counter(current_level) = 0;
        current_repeat_skip(current_level) = 0;
        current_repeat_count(current_level) = sz;
        
        current_start = current_start + 1;
        if current_level==0
            repeat_count(i) = 1;
        else
            repeat_count(i) = prod(current_repeat_count(1:current_level));
            repeat_count_all(i,1:current_level) = current_repeat_count(1:current_level);
            current_repeat_counter(current_level) = current_repeat_counter(current_level) + 1;
            current_repeat_skip(current_level) = current_repeat_skip(current_level) + 1;
        end
        repeat_first(i) = current_start;
        repeat_level(i) = current_level;

        true_repeat_count = prod(current_repeat_count(1:current_level));
        repeat_len = current_repeat_counter(current_level);
        repeat_skip(i,current_level) = current_repeat_skip(current_level);
        current_start = current_start + current_repeat_skip(current_level)*(current_repeat_count(current_level)-1);
        if current_level>1
            current_repeat_skip(current_level-1) = current_repeat_skip(current_level-1)+current_repeat_count(current_level)*current_repeat_counter(current_level);
            current_repeat_counter(current_level-1) = current_repeat_counter(current_level-1)+current_repeat_count(current_level)*current_repeat_counter(current_level);
        end
        current_level = current_level - 1;
    end
%     fprintf('%d ',current_repeat_count);
%     fprintf(' \t');
%     fprintf('%d ',current_repeat_counter);
%     fprintf(' \t');
%     fprintf('%d ',current_repeat_skip);
%     fprintf('\n');
end

%% check for dublicate entries
[~,fname,ext]=fileparts(filename);
is_indexed_bin = ~isempty(strfind(fname, 'RadarParams')) ...
              || ~isempty(strfind(fname, 'Detections')) ...
              || ~isempty(strfind(fname, 'TrackerClutterRegion')) ...
              || ~isempty(strfind(fname, 'DetectionClusters')) ...
              || ~isempty(strfind(fname, 'Tracklets'));
for i=1:length(defs)
    compresult = strcmp(defs(i), defs);
    if strcmp(defs(i), 'index') && ~is_indexed_bin
        defs{i} = [defs{i} '_rename'];
        warnmsg = ['The file ' fname ext ' contains a variable called index. Since this will cause problems using ASGUI it has been renamed to ''index_rename''.'];
        warning(warnmsg);
%        questdlg(warnmsg, 'bin file import issure', 'I''ll check the resim', 'I''ll check the resim');
    end
    if ~strcmp(defs(i), 'END_REPEAT') && sum(compresult) > 1 && (length(defs{i})>6 && ~strcmp(defs{i}(1:6), 'REPEAT'))
        % this entry is in the list more then once and not a REPEAT
        % statement
        
        %find the second instance
        second_index = i + find(compresult(i+1:end)==1, 1, 'first');
        new_name = [defs{second_index} '_rename'];
        %rename second instance
        defs{second_index} = new_name; 
        % give some user feedback
        warnmsg = ['The signal "' defs{i} '" is dublicate in the file ' fname ext '. Renaming second one to ' new_name];
        warning(warnmsg);
 %       questdlg(warnmsg, 'bin file import issure', 'I''ll check the resim', 'I''ll check the resim');
    end
end
% extract binary data
if ~enable_size
data_start = ftell(fid);
fseek(fid,0,'eof');
data_end = ftell(fid);
data_len = data_end-data_start;
fseek(fid,data_start,'bof');
recordsize = file_precision_byte_size*sum(repeat_count);
nrecs = data_len / recordsize;
if round(nrecs) ~= nrecs
    warnmsg = ['Corrupted data in file ''' filename '''! Did the resim break before finishing?'];
    warning(warnmsg);
    nrecs = floor(nrecs);
end

try
    X = fread(fid,[recordsize/file_precision_byte_size,nrecs],file_precision);%######file_precision######
catch
    nrecs = 0;
    memory
end

fclose(fid);

% parse into structure
if nrecs==0
    warnmsg = ['No data in file ''' filename '''!'];
    warning(warnmsg);
    if aos_flag
        data = [];
    else
        data.scanIndex = [];
    end
else
    if aos_flag
        for i = 1:num_defs
            if valid(i)
                temp_ind = repeat_first(i);
                for j = repeat_level(i):-1:1
                    temp_ind = temp_ind'*ones(1,repeat_count_all(i,j)) + ones(length(temp_ind),1)*(repeat_skip(i,j)*(0:repeat_count_all(i,j)-1));
                    temp_ind = reshape(temp_ind',1,[]);
                end
                temp_data = X(temp_ind,:)';
                if repeat_level(i)>0
                    temp_data = reshape(temp_data,[nrecs repeat_count_all(i,1:repeat_level(i))]);
                end
                temp_data_cell = mat2cell(temp_data,ones(size(temp_data,1),1));
                [data(1:length(temp_data_cell)).(defs{i})] = temp_data_cell{:};
            end
        end
    else
        for i = 1:num_defs
            if valid(i)
                temp_ind = repeat_first(i);
                for j = repeat_level(i):-1:1
                    temp_ind = temp_ind'*ones(1,repeat_count_all(i,j)) + ones(length(temp_ind),1)*(repeat_skip(i,j)*(0:repeat_count_all(i,j)-1));
                    temp_ind = reshape(temp_ind',1,[]);
                end
                if repeat_level(i)==0
                    data.(defs{i}) = X(temp_ind,:)';
                else
                    temp_data= X(temp_ind,:)';
                    data.(defs{i})= reshape(temp_data,[nrecs repeat_count_all(i,1:repeat_level(i))]);
                end
            end
        end
    end
end
[path,fName,fExt] = fileparts(filename);
[path, resim, ~] = fileparts(path);
[~, folder, ~] = fileparts(path);
c = filesep;
if isempty(warnmsg) && silent == 0
    disp(['"' folder c resim c fName fExt '" loaded after ' num2str(toc) 'seconds']);
end
else
    
data_start = ftell(fid);
fseek(fid,0,'eof');
data_end = ftell(fid);
data_len = data_end-data_start;
fseek(fid,data_start,'bof');
recordsize = sum(repeat_count);
nrecs = data_len / recordsize;

X = fread(fid,[recordsize,nrecs],'*uint8');

for i = 1:num_defs
    if valid(i)
        temp_ind = repeat_first(i);
        for j = repeat_level(i):-1:1
            temp_ind = temp_ind'*ones(1,repeat_count_all(i,j)) + ones(length(temp_ind),1)*(repeat_skip(i,j)*(0:repeat_count_all(i,j)-1));
            temp_ind = reshape(temp_ind',1,[]);
        end
        if repeat_level(i)==0
            temp_data = X(temp_ind,:)';
        else
            temp_data= X(temp_ind,:)';
            temp_data= reshape(temp_data,[nrecs repeat_count_all(i,1:repeat_level(i))]);
        end
        temp_data = permute(temp_data,[ndims(temp_data) 1:ndims(temp_data)-1]);
        if repeat_level(i)==1
            data.(defs{i}) = typecast(temp_data(:),types{i});
        else
            data.(defs{i}) = reshape(typecast(temp_data(:),types{i}),[nrecs repeat_count_all(i,1:repeat_level(i)-1)]);
        end
    end
end
fclose(fid);
if isempty(warnmsg) && silent == 0
    disp(['"' filename '" loaded in ' num2str(toc) 'seconds']);
end
end

