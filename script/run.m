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

%% Learn predict and output
y2 = learnAndPredict(X, y, X2, 1); % decision tree
tic;
fprintf('Output... ');
output = [valid_data(:,1:(size(valid_data,2)-1)) y2];
dlmwrite(sprintf('%s\\Valid.feature.predict.csv', DATA_DIRECTORY), output, 'delimiter', ',', 'precision', 10);
fprintf('%f s\n', toc);

%% Unfold result and calculate MAP score
system('python ..\script\result.py ..\data\Valid.feature.predict.csv');
system('python ..\script\eval.py ..\data\Valid.feature.predict.result.csv ..\data\ValidSolution.csv');
