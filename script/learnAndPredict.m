function [ y2 ] = learnAndPredict( X, y, X2, method )
%LEARN Summary of this function goes here
%   Detailed explanation goes here
%% Train
tic;
fprintf('Training... ');
if method == 1 % Decision tree
    ctree = compact(fitctree(X,y));
    % view(ctree, 'mode', 'graph');
elseif method == 2 % SVM (RBF kernel)
    svm = fitcsvm(X,y,'KernelFunction','rbf','Standardize',true);
else
    error('method error!');
end
fprintf('%f s\n', toc);

%% Predict
tic;
fprintf('Predicting... ');
if method == 1 % Decision Tree
    y2 = ctree.predict(X2);
elseif method == 2 % SVM (RBF kernel)
    y2 = svm.predict(X2);
else
    error('method error!');
end
fprintf('%f s\n', toc);


end

