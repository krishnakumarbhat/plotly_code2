function data = read_log_scalar_field(filename, fieldname, show_warnings)

data = [];

if nargin < 3
    show_warnings = true;
end

fid = fopen(filename,'rb');
if fid==-1
    if show_warnings
        warning(['Could not open file: ' filename]);
    end
    return;
end

if(strfind(filename, '.bin32') >0)
    file_precision = 'float32';
else
    file_precision = 'float64';
end

dir_data = dir(filename);
if dir_data.bytes == 0
    data = [];
    warnmsg = ['Empty file ''' filename '''!'];
    warning(warnmsg);
    return;
end

MAX_LEVELS = 6;

% process header info
num_defs = fread(fid,1,file_precision);
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
        end
        current_level = current_level - 1;
    else
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
        repeat_skip(i) = 0;
        repeat_first(i) = current_start;
        repeat_level(i) = current_level;
    end
end

% determine data start and number of records
data_start = ftell(fid);
fseek(fid,0,'eof');
data_end = ftell(fid);
data_len = data_end-data_start;
recordsize = 8*sum(repeat_count);
entry_size = 8;
if(strcmp(file_precision , 'float32'))
    recordsize = 4*sum(repeat_count);
    entry_size = 4;
end
nrecs = data_len / recordsize;
if round(nrecs) ~= nrecs
    if show_warnings
        warning(['Corrupted data in file: ' filename]);
    end
    nrecs= floor(nrecs);
end
% extract selected field
if nrecs==0
    if show_warnings
        warning(['No data in file: ' filename]);
    end
else
    i = find(strcmp(defs,fieldname),1);
    if isempty(i)
        if show_warnings
            warning(['Field ' fieldname ' not found in file: ' filename]);
        end
    else
        fseek(fid,data_start + (i - 1)*entry_size,'bof');
        data = fread(fid,nrecs,file_precision,recordsize - entry_size);
    end
end

fclose(fid);
