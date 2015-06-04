FEATURE_EXTRACTOR_EXE_PATH = '..\FeatureExtractor\x64\Release\FeatureExtractor.exe';
TRAIN_FEATURE_PATH = '..\data\Train.feature.csv';
DATA_DIRECTORY = '..\data';

%% Extract features
delete('..\data\*.feature.*');
tic;
fprintf('Extracting features... ');
system(sprintf('%s %s', FEATURE_EXTRACTOR_EXE_PATH, DATA_DIRECTORY));
fprintf('%f s\n', toc);

%% Load feature data
train_data = csvread(sprintf('%s\\Train.feature.csv', DATA_DIRECTORY), 1);
valid_data = csvread(sprintf('%s\\Valid.feature.csv', DATA_DIRECTORY), 1);
X = train_data(:, 3:(size(train_data,2)-1));
y = train_data(:, end);
X2 = valid_data(:, 3:(size(valid_data,2)-1));

% X = basis_expansion(X);
% X2 = basis_expansion(X2);

%% Learn predict and output
[y1, y2, model] = learnAndPredict(X, y, X2, 2); % decision tree
tic;
fprintf('Training Output... ');
fp_predict = fopen(sprintf('%s\\Train.feature.predict.csv', DATA_DIRECTORY), 'w');
fprintf(fp_predict, 'author_id,paper_id,features\n');
dlmwrite(sprintf('%s\\Train.feature.predict.csv', DATA_DIRECTORY), [train_data(:,1:(size(train_data,2)-1)) y1], 'delimiter', ',', 'precision', 10, '-append');
fprintf('%f s\n', toc);
fclose(fp_predict);

tic;
fprintf('Valid Output... ');
fp_predict = fopen(sprintf('%s\\Valid.feature.predict.csv', DATA_DIRECTORY), 'w');
fprintf(fp_predict, 'author_id,paper_id,features\n');
dlmwrite(sprintf('%s\\Valid.feature.predict.csv', DATA_DIRECTORY), [valid_data(:,1:(size(valid_data,2)-1)) y2], 'delimiter', ',', 'precision', 10, '-append');
fprintf('%f s\n', toc);
fclose(fp_predict);

%% Unfold result and calculate MAP score
system('python ..\script\result.py ..\data\Train.feature.predict.csv');
system('python ..\script\result.py ..\data\Valid.feature.predict.csv');
system('python ..\script\eval.py ..\data\Train.feature.predict.result.csv ..\data\Train.csv');
system('python ..\script\eval.py ..\data\Valid.feature.predict.result.csv ..\data\ValidSolution.csv');
