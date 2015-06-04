function [ y1, y2, model ] = learnAndPredict( X, y, X2, method )
%LEARN Summary of this function goes here
%   Detailed explanation goes here
%% Train
tic;
fprintf('Training... ');
if method == 1 % Decision tree
    model = fitctree(X,y);
    % view(model, 'mode', 'graph');
elseif method == 2 % SVM (RBF kernel)
    model = fitcsvm(X,y,'KernelFunction','rbf','Standardize',true,'KernelScale','auto');
elseif method == 3 % KNN
    model = fitcknn(X,y, 'NumNeighbors', 5);
else
    error('method error!');
end
fprintf('%f s\n', toc);

%% Predict
tic;
fprintf('Predicting... ');
if method == 1 % Decision Tree
    y1 = model.predict(X);
    y2 = model.predict(X2);
elseif method == 2 % SVM (RBF kernel)
    y1 = model.predict(X);
    y2 = model.predict(X2);
elseif method == 3 %KNN
    y1 = model.predict(X);
    y2 = model.predict(X2);
else
    error('method error!');
end
fprintf('%f s\n', toc);


end

