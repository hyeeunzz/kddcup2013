function [X,y,X2,y1,y3,model] = run(params)
DATA_DIRECTORY = '../data';

%% Experimental settings..
algorithm = params.algorithm;                       % 0: logistic regression, 1: decision tree, 2: kernel SVM, 3: KNN
preprocessing_method = params.preprocessing_method; % Preprocessing method : 'Z' or 'minmax'
postprocessing_critical = params.postprocessing_critical; % Whether using critical feature as postprocessing or not
feature_list = params.feature_list; % Feature ids that will be used for training and prediction
be = params.be; % basis expansion?
fprintf('algorithm : %d\n', algorithm);
fprintf('preprocessing : %s\n', preprocessing_method);
fprintf('postprocessing? : %d\n', postprocessing_critical);
fprintf('basis_exp : %d\n', be);

%% Extract features
% delete('..\data\*.predict.*');
% tic;
% fprintf('Extracting features... ');
% system(sprintf('%s %s', FEATURE_EXTRACTOR_EXE_PATH, DATA_DIRECTORY));
% fprintf('%f s\n', toc);

%% Load feature data
fprintf('feature list : ');
fprintf('%d ', feature_list);
fprintf('\n');

train_data = loadFeature('Train', feature_list);
valid_data = loadFeature('Valid', feature_list);
test_data = loadFeature('Test', feature_list);
%{
R = randperm(size(train_data, 1));
train_data = train_data(R, :);
train_data = sortrows(train_data, 1);
%}

X = train_data(:, 3:(size(train_data,2)-1));
y = train_data(:, end);
X2 = valid_data(:, 3:(size(valid_data,2)-1));
X3 = test_data(:, 3:(size(test_data,2)-1));

if strcmp(preprocessing_method, 'Z') || strcmp(preprocessing_method, 'minmax')
  X  = preprocessing(X, preprocessing_method);
  X2 = preprocessing(X2, preprocessing_method);
  X3 = preprocessing(X3, preprocessing_method);
end

%sz = 10000;
X = [X ; X2];
y = [y ; valid_data(:, end)];
fprintf('train size : %d\n', size(X,1));

%% Learn predict and output
[y1, y3, model] = learnAndPredict(X, y, X3, algorithm);

tic;
fprintf('Test Output... ');
fp_predict = fopen(sprintf('%s/Test.predict.%s.csv', DATA_DIRECTORY, preprocessing_method), 'w');
fprintf(fp_predict, 'author_id,paper_id,features\n');
dlmwrite(sprintf('%s/Test.predict.%s.csv', DATA_DIRECTORY, preprocessing_method), [test_data(:,1:(size(test_data,2)-1)) y3], 'delimiter', ',', 'precision', 10, '-append');
fprintf('%f s\n', toc);
fclose(fp_predict);

%% Unfold result and calculate MAP score
system(sprintf('python ../script/result.py ../data/Test.predict.%s.csv %d', preprocessing_method, postprocessing_critical));
end

