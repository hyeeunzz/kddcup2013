function [ X2 ] = preprocessing( X, method )
%PREPROCESSING Summary of this function goes here
%   Detailed explanation goes here

if strcmp(method, 'minmax')
    X2 = (X - repmat(min(X), size(X, 1), 1)) ./ repmat(max(X)-min(X), size(X, 1), 1); X2(isnan(X2)) = 0;
elseif strcmp(method, 'Z')
    X2 = (X - repmat(mean(X), size(X, 1), 1)) ./ repmat(std(X), size(X, 1), 1); X2(isnan(X2)) = 0;
else
    error('Preprocessing method error');
end

end

