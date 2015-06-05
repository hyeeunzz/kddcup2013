function [ data ] = loadFeature( dataname, feature_list )
%LOADFEATURE Summary of this function goes here
%   Detailed explanation goes here

data = [];
for i=1:length(feature_list)
    feature_id = feature_list(i);
    tmp = csvread(sprintf('../data/%s.feature.%d.csv', dataname, feature_id), 1);
    if i == 1
        author_ids = tmp(:, 1);
        paper_ids = tmp(:, 2);
        y = tmp(:, 4);
    end
    data = [data tmp(:, 3)];
end

data = [author_ids paper_ids data y];

end

