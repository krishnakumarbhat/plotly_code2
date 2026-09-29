function write_log_data(filename, data)
% currently this can only write non array data

if isempty(data)
    return;
end

try
    file_precision = 'float64';
    file_precision_byte_size = 8;
    
    fnames = fieldnames(data);
    num_defs = length(fnames);
    fid = fopen(filename,'wb');
    fwrite(fid,[num_defs],file_precision);
    data_written = false(size(fnames));
    data_order = ones(size(fnames));
    data_counter = 1;
    
    count_headers = 0;
    % write higher dimensional data
    for idata=1:length(fnames)
        if ~data_written(idata)
            [~,b] = size(data.(fnames{idata}));
            if b == 1
                name = [fnames{idata} zeros(1, 256-length(fnames{idata}))];
                fwrite(fid,name,'uchar');
                count_headers = count_headers +1;
                data_written(idata) = true;
                data_order(data_counter) = idata;
                data_counter = data_counter + 1;
            else
                name = ['REPEAT' num2str(b)];
                name = [name zeros(1, 256-length(name))];
                fwrite(fid,name,'uchar');
                count_headers = count_headers +1;
                for j=idata:length(fnames)
                    [~,b2] = size(data.(fnames{j}));
                    if b==b2
                        name = [fnames{j} zeros(1, 256-length(fnames{j}))];
                        fwrite(fid,name,'uchar');
                        count_headers = count_headers +1;
                        data_written(j) = true;
                        data_order(data_counter) = j;
                        data_counter = data_counter + 1;
                    end
                end
                name = 'END_REPEAT';
                name = [name zeros(1, 256-length(name))];
                fwrite(fid,name,'uchar');
                count_headers = count_headers +1;
            end
        end
    end
    
    pos = ftell(fid);
    fseek(fid, 0,-1);
    fwrite(fid,count_headers,file_precision);
    fseek(fid, pos,-1);
    
    for idata=1:length(data.(fnames{1}))
        data_written = false(size(fnames));
        % write 1D data first
        for idata_written=1:length(data_written)
            if ~data_written(idata_written)
                [~,b] = size(data.(fnames{idata_written}));
                if b== 1
                    fwrite(fid,data.(fnames{idata_written})(idata),file_precision);
                    data_written(idata_written) = true;
                else
                    for k = 1:b %all array elements
                        for j2=idata_written:length(fnames)
                            [~,b2] = size(data.(fnames{j2}));
                            if b==b2
                                fwrite(fid,data.(fnames{j2})(idata,k),file_precision);
                                data_written(j2) = true;
                            end
                        end
                    end
                end
            end
        end
    end
    fclose(fid);
catch ME
    if (exist('fid', 'var') == 1)
        if (fid ~= -1)
            fclose(fid);
        end
    end
end
