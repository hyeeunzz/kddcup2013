% Logistic regression
[X,y,X2,y1,y2,model] = run(struct( ...
'algorithm', 2, ...
'preprocessing_method', 'Z', ...
'postprocessing_critical', 0, ...
'feature_list', [101:117 200:205 206 207 208:217], ...
'be', 0 ... % basis expansion
));

% Linear SVM
%{
fprintf('\n=============================\n');
[X,y,X2,y1,y2,model] = run(struct( ...
'algorithm', 4, ...
'preprocessing_method', 'Z', ...
'postprocessing_critical', 0, ...
'feature_list', [101:117 200:217], ...
'be', 0 ... % basis expansion
));
%}

% Logistic regression with basis expansion
%{
fprintf('\n=============================\n');
[X,y,X2,y1,y2,model] = run(struct( ...
'algorithm', 2, ...
'preprocessing_method', 'Z', ...
'postprocessing_critical', 0, ...
'feature_list', [101:117 200:217], ...
'be', 0 ... % basis expansion
));
%}

% kernel SVM
%feature=[101:117 200:217];
%feature = [101 102 103 104 108 109 110 111];
%feature = [301];

%{
[X,y,X2,y1,y2,model] = run(struct( ...
'algorithm', 2, ...
'preprocessing_method', 'minmax', ...
'postprocessing_critical', 0, ...
'feature_list', feature, ...
'be', 0 ... % basis expansion
));
%}

%{
[X,y,X2,y1,y2,model] = run3(struct( ...
'algorithm', 2, ...
'preprocessing_method', 'minmax', ...
'postprocessing_critical', 1, ...
'feature_list', feature, ...
'be', 0 ... % basis expansion
));

%}
