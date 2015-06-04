function [ conference_ids, matrix ] = loadData( filepath )
%LOADDATA Summary of this function goes here
%   Detailed explanation goes here
fprintf('path : %s\n', filepath);

fp = fopen(filepath, 'rb');

if fp == -1
    disp('file cannot be found.');
end

N = fread(fp, 1, 'int32');
fprintf('N : %d\n', N);
conference_ids = zeros(N,1);
for i=1:N
    conference_ids(i) = fread(fp, 1, 'int32');
end
matrix = fread(fp, [N, N], 'int32');

fclose(fp);

end
