% This script runs a simple example of latexTable.m
clc; clear input;

%% Example 1: using an array as data input
fprintf('Example 1: using an array as data input\n\n');

% row labels (set empty string for multiline rows):
input.tableRowLabels = {'a','b','','c'};

% numeric values you want to tabulate:
% this field has to be an array or a MATLAB table
% in this example we use an array
input.tableData = [5.30856975723896,5.63030125767768,5.74729453056449;...
    3.00701884004433,3.13261913557444,3.16956039896564;...
    2.09691760521636,2.17842323651452,1.78982547344968];

% Optional fields (if not set default values will be used):

% column labels:
input.tableCloumnHeaders = {'Axle Load 7.4t','Axle Load 5.25t','Axle Load 4.5t'};

% Formatting-string to set the precision of the table values:
% For using a single format for all values in input.tableData use
% just one format string in the cell array like {myFormatString}
% e.g. input.tableDataRowFormat = {'%.3f'};
% For using different formats in different rows use a cell array like
% {myFormatString1,numberOfRows1,myFormatString2,numberOfRows2, ... }
% where myFormatString_ are formatting-strings and numberOfRows_ are the 
% number of table rows that the preceding formatting-string applies.
% Make sure the sum of numberOfRows_ matches the number of rows in input.tableData!
% For further information about formatting strings see 
% http://www.mathworks.de/de/help/matlab/matlab_prog/formatting-strings.html
% In this example we want to use a different format for the last row:
input.tableDataRowFormat = {'%.2f',3,'%.2f',1};

% Define how NaN values in input.tableData should be printed in the LaTex table:
input.tableDataNanString = '-';

% Column alignment ('l'=left-justified, 'c'=centered,'r'=right-justified):
input.tableColumnAlignment = 'c';

% Switch table borders on/off:
input.tableBorders = 1; 

% LaTex table caption:
input.tableCaption = 'MyTableCaption';

% LaTex table label:
input.tableLabel = 'MyTableLabel';

% Switch to generate a complete LaTex document or just a table:
input.makeCompleteLatexDocument = 1;

% Now call the function to generate LaTex code:
latex = latexTable(input);



%% Example 2: using MATLAB table datatype as data input

% Please note: since the table datatype was introduced in MATLAB version r2013b,
% you cannot use this feature in older versions of MATLAB!
% Check MATLAB version:
DateNumberThisVersion = datenum(version('-date'),'mmmm dd, yyyy');
if DateNumberThisVersion < 735459 % MATLAB r2013b release day was datenumber 735459
    fprintf('\n\nCannot run example 2: This MATLAB version does not support datatype ''table''!\n');
    break;
end

fprintf('\n\nExample 2: using MATLAB table datatype as data input\n\n');

% Set up a MATLAB table (similar to example used in MATLAB docs):
% Please note that the resulting LaTex table is row-based, not
% column-based. So the LaTex table is a 'transposed' copy of the MATLAB table.
LastName = {'Smith';'Johnson';'Williams';'Jones';'Brown'};
Age = [38;43;38;40;49];
Height = [71;69;64;67;64];
Weight = [176;163;131;133;119];
T = table(Age,Height,Weight,'RowNames',LastName);

% Now use this table as input in our input struct:
input.tableData = T;

% Set the row format of the data values (in this example we want to use
% integers only):
input.tableDataRowFormat = {'%i'};

% Column alignment ('l'=left-justified, 'c'=centered,'r'=right-justified):
input.tableColumnAlignment = 'c';

% Switch table borders on/off:
input.tableBorders = 1; 

% Switch to generate a complete LaTex document or just a table:
input.makeCompleteLatexDocument = 1;

% Now call the function to generate LaTex code:
latex = latexTable(input);