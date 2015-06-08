function [ y1, y2, model ] = learnAndPredict( X, y, X2, method )
%LEARN Summary of this function goes here
%   Detailed explanation goes here
%% Train
tic;
fprintf('Training(method %d)... ', method);
if method == 0 %logistic regression
    model = glmfit(X, (y+1)/2, 'binomial', 'link', 'logit');
elseif method == 1 % Decision tree
    model = fitctree(X,y);
    % view(model, 'mode', 'graph');
elseif method == 2 % SVM (RBF kernel)
    model = fitcsvm(X,y,'KernelFunction','rbf','Standardize',true,'KernelScale','auto');
elseif method == 3 % KNN
    model = fitcknn(X,y, 'NumNeighbors', 5);
elseif method == 4 % SVM (linear kernel)
    model = fitcsvm(X,y,'KernelFunction','linear','Standardize',true,'KernelScale','auto');
else
    error('method error!');
end
fprintf('%f s\n', toc);

%% Predict
tic;
fprintf('Predicting... ');
if method == 0 %logistic regression
    Z1 = model(1) + X*model(2:size(model,1));
    y1 = 1 ./ (1 + exp(-Z1));
    y1 = 2*(y1 > 0.5)-1;
    Z2 = model(1) + X2*model(2:size(model,1));
    y2 = 1 ./ (1 + exp(-Z2));
    y2 = 2*(y2 > 0.5)-1;
elseif method == 1 % Decision Tree
    y1 = model.predict(X);
    y2 = model.predict(X2);
elseif method == 2 % SVM (RBF kernel)
    y1 = model.predict(X);
    y2 = model.predict(X2);
elseif method == 3 %KNN
    y1 = model.predict(X);
    y2 = model.predict(X2);
elseif method == 4 % SVM (linear kernel)
    y1 = model.predict(X);
    y2 = model.predict(X2);
else
    error('method error!');
end
fprintf('%f s\n', toc);


end

