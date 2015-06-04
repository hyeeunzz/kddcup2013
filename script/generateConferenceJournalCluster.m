FEATURE_EXTRACTOR_EXE_PATH = '..\FeatureExtractor\x64\Release\FeatureExtractor.exe';
DATA_DIRECTORY = '..\data';
k = 25; % # of clusters

%% Run FeatureExtractor.exe with parameter
tic;
fprintf('Generating Conference & Journal Similarity matrix...\n');
system(sprintf('%s %s -cj', FEATURE_EXTRACTOR_EXE_PATH, DATA_DIRECTORY));
fprintf('%f s\n', toc);

%% CONFERENCE : Load data and construct graph Laplacian
fprintf('Conference: construct graph Laplacian...\n');
[ids, X] = loadData(sprintf('%s\\ConferenceSimilarity.dat', DATA_DIRECTORY));
X = log(X*100 + 1) + 0.3;

d = sum(X,2);
d_inv = 1 ./ d;
L = diag(d) - X;
L = repmat(d_inv, 1, length(X)) .* L;

%% CONFERENCE : Perform spectral clustering
tic;
fprintf('SVD... ');
[U,S,V] = svds(L, k);
fprintf('%f s\n', toc);
tic;
fprintf('Kmeans... ');
result = kmeans(U, k);
fprintf('%f s\n', toc);

%% CONFERENCE : save results
counts = accumarray(result, ones(size(result)), [], @sum); disp(counts);
fp_cc = fopen(sprintf('%s\\ConferenceCluster.csv', DATA_DIRECTORY), 'w');
fprintf(fp_cc, 'conference_id,cluster\n');
dlmwrite(sprintf('%s\\ConferenceCluster.csv', DATA_DIRECTORY), [ids result], 'delimiter', ',', 'precision', 10, '-append');
fclose(fp_cc);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% JOURNAL : Load data and construct graph Laplacian
fprintf('Journal: construct graph Laplacian...\n');
[ids, X] = loadData(sprintf('%s\\JournalSimilarity.dat', DATA_DIRECTORY));
X = log(X*100 + 1) + 0.3;

d = sum(X,2);
d_inv = 1 ./ d;
L = diag(d) - X;
L = repmat(d_inv, 1, length(X)) .* L;

%% JOURNAL : Perform spectral clustering
tic;
fprintf('SVD... ');
[U,S,V] = svds(L, k);
fprintf('%f s\n', toc);
tic;
fprintf('Kmeans... ');
result = kmeans(U, k);
fprintf('%f s\n', toc);

%% JOURNAL : save results
counts = accumarray(result, ones(size(result)), [], @sum);
disp(counts);
fp_cc = fopen(sprintf('%s\\JournalCluster.csv', DATA_DIRECTORY), 'w');
fprintf(fp_cc, 'journal_id,cluster\n');
dlmwrite(sprintf('%s\\JournalCluster.csv', DATA_DIRECTORY), [ids result], 'delimiter', ',', 'precision', 10, '-append');
fclose(fp_cc);