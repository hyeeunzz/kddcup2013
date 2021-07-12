function [X,y,X2,y1,y2,model] = run(params)
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
R = randperm(size(train_data, 1));
train_data = train_data(R, :);
train_data = sortrows(train_data, 1);

X = train_data(:, 3:(size(train_data,2)-1));
y = train_data(:, end);
X2 = valid_data(:, 3:(size(valid_data,2)-1));

X  = preprocessing(X, preprocessing_method);
X2 = preprocessing(X2, preprocessing_method);

if be
    X = basis_expansion(X);
    X2 = basis_expansion(X2);
end

sz = 235909;
%sz = 10000;
X = X(1:sz, :);
y = y(1:sz, :);
fprintf('train size : %d\n', size(X,1));

%% Learn predict and output
suffix = sprintf('predict.sz%d', sz);
[y1, y2, model] = learnAndPredict(X, y, X2, algorithm);
tic;
fprintf('Training Output... ');
fp_predict = fopen(sprintf('%s/Train.%s.csv', DATA_DIRECTORY, suffix), 'w');
fprintf(fp_predict, 'author_id,paper_id,features\n');
dlmwrite(sprintf('%s/Train.%s.csv', DATA_DIRECTORY, suffix), [train_data(1:sz,1:(size(train_data,2)-1)) y1], 'delimiter', ',', 'precision', 10, '-append');
fprintf('%f s\n', toc);
fclose(fp_predict);

tic;
fprintf('Valid Output... ');
fp_predict = fopen(sprintf('%s/Valid.%s.csv', DATA_DIRECTORY, suffix), 'w');
fprintf(fp_predict, 'author_id,paper_id,features\n');
dlmwrite(sprintf('%s/Valid.%s.csv', DATA_DIRECTORY, suffix), [valid_data(:,1:(size(valid_data,2)-1)) y2], 'delimiter', ',', 'precision', 10, '-append');
fprintf('%f s\n', toc);
fclose(fp_predict);

%% Unfold result and calculate MAP score
system(sprintf('python ../script/result.py ../data/Train.%s.csv %d', suffix, postprocessing_critical));
system(sprintf('python ../script/result.py ../data/Valid.%s.csv %d', suffix, postprocessing_critical));
system(sprintf('python ../script/eval.py ../data/Train.%s.result.csv ../data/Train.csv', suffix));
system(sprintf('python ../script/eval.py ../data/Valid.%s.result.csv ../data/ValidSolution.csv', suffix));
system(sprintf('python ../script/error.py ../data/Train.%s.csv ../data/Train%d.csv', suffix, sz));
system(sprintf('python ../script/error.py ../data/Valid.%s.csv ../data/ValidSolution.csv', suffix));
end
