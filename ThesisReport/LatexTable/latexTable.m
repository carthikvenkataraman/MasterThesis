function latex = latexTable(input)
% An easy to use function that generates a LaTeX table from a given MATLAB
% input struct containing numeric values. The LaTeX code is printed in the 
% command window for quick copy&paste and given back as a cell array.
%
% Author:   Eli Duenisch
% Date:     November 21, 2013
%
% Input:
% input    struct containing mandatory and optional fields (details described below)
%
% Output:
% latex    cell array containing LaTex code
%
% Example and explanation of the input struct fields:
%
% % row labels (set empty string for multiline rows):
% input.tableRowLabels = {'a','b','','c'};
%
% % numeric values you want to tabulate:
% % this field has to be an array or a MATLAB table
% % in this example we use an array
% input.tableData = [1.12345 2.12345 3.12345; ... 
%                    4.12345 5.12345 6.12345; ...
%                    7.12345 8.12345 9.12345; ...
%                    10.12345 11.12345 12.12345];
%
% % Optional fields (if not set default values will be used):
%
% % Set column labels:
% input.tableCloumnHeaders = {'column1','column2','column3'};
%
% % Formatting-string to set the precision of the table values:
% % For using a single format for all values in input.tableData use
% % just one format string in the cell array like {myFormatString}
% % e.g. input.tableDataRowFormat = {'%.3f'};
% % For using different formats in different rows use a cell array like
% % {myFormatString1,numberOfRows1,myFormatString2,numberOfRows2, ... }
% % where myFormatString_ are formatting-strings and numberOfRows_ are the 
% % number of table rows that the preceding formatting-string applies.
% % Make sure the sum of numberOfRows_ matches the number of rows in input.tableData!
% % For further information about formatting strings see 
% % http://www.mathworks.de/de/help/matlab/matlab_prog/formatting-strings.html
% % In this example we want to use a different format for the last row:
% input.tableDataRowFormat = {'%.3f',3,'%.2f',1};
%
% % Define how NaN values in input.tableData should be printed in the LaTex table:
% input.tableDataNanString = '-';
%
% % Column alignment ('l'=left-justified, 'c'=centered,'r'=right-justified):
% input.tableColumnAlignment = 'c';
%
% % Switch table borders on/off:
% input.tableBorders = 1; 
%
% % LaTex table caption:
% input.tableCaption = 'MyTableCaption';
%
% % LaTex table label:
% input.tableLabel = 'MyTableLabel';
%
% % Switch to generate a complete LaTex document or just a table:
% input.makeCompleteLatexDocument = 0;
%
% % Now call the function to generate LaTex code:
% latex = latexTable(input);

%%%%%%%%%%%%% Default settings (used if optional inputs are not given)%%%%%
% Sets the default display format of numeric values in the LaTeX table to '%.4f' f
% (4 digits floating point precision).
if ~isfield(input,'tableDataRowFormat'),input.tableDataRowFormat = {'%.2f'};end
% Define what should happen with NaN values in input.tableData:
if ~isfield(input,'nanString'),input.tableDataNanString = '-';end
% Specify the alignment of the columns:
% 'l' for left-justified, 'c' for centered, 'r' for right-justified
if ~isfield(input,'tableColumnAlignment'),input.tableColumnAlignment = 'c';end
% Specify whether the table has borders:
% 0 for no borders, 1 for borders
if ~isfield(input,'tableBorders'),input.tableBorders = 1;end
% Other optional fields:
if ~isfield(input,'tableCaption'),input.tableCaption = 'MyTableCaption';end
if ~isfield(input,'tableLabel'),input.tableLabel = 'MyTableLabel';end
if ~isfield(input,'makeCompleteLatexDocument'),input.makeCompleteLatexDocument = 0;end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% process table datatype
if strcmp(class(input.tableData),'table')
    input.tableCloumnHeaders = input.tableData.Properties.RowNames';
    input.tableRowLabels = input.tableData.Properties.VariableNames';
    input.tableData = table2array(input.tableData)';
end
% get size
numberDataCols = size(input.tableData,2);
numberDataRows = size(input.tableData,1);
% make table header lines:
hLine = '\hline';
if input.tableBorders
    header = ['\begin{tabular}{|',repmat([input.tableColumnAlignment,'|'],1,numberDataCols+1),'}'];
else
    header = ['\begin{tabular}{',repmat(input.tableColumnAlignment,1,numberDataCols+1),'}'];
end
latex = {'\begin{table}[H!]';'\centering';header};
% generate column headers
if isfield(input,'tableCloumnHeaders')
    if input.tableBorders,latex(end+1) = {hLine};end
    rowStr = ' ';
    for i=1:length(input.tableCloumnHeaders)
        rowStr = [rowStr,' & ',input.tableCloumnHeaders{i}];
    end
    latex(end+1) = {[rowStr,' \\']};
end
% generate row-format cell array based on input.tableDataFormat
tableDataRowFormatList={};
lengthTableDataRowFormat = length(input.tableDataRowFormat);
if lengthTableDataRowFormat==1
    tableDataRowFormatList = repmat(input.tableDataRowFormat(1),numberDataRows,1);
else    
    for i=1:2:lengthTableDataRowFormat
        for j=1:input.tableDataRowFormat{i+1}
            tableDataRowFormatList(end+1,1) = input.tableDataRowFormat(i);
        end
    end
end
% make table rows:
for i=1:numberDataRows
    rowHeader = input.tableRowLabels{i};
    if ~isempty(rowHeader) && input.tableBorders
        latex(end+1) = {hLine};
    end
    rowStr = rowHeader;
    for j=1:numberDataCols
        dataValue = input.tableData(i,j);
        if isnan(dataValue)
            dataValue = input.tableDataNanString;
        else
            dataValue = num2str(dataValue,tableDataRowFormatList{i});
        end
        rowStr = [rowStr,' & ',dataValue];     
    end
    latex(end+1) = {[rowStr,' \\']};
end
% make table footer lines:
footer = {'\end{tabular}';['\caption{',input.tableCaption,'}']; ...
          ['\label{table:',input.tableLabel,'}'];'\end{table}'};
if input.tableBorders
    latex = [latex;{hLine};footer];
else
    latex = [latex;footer];  
end
% add code if a complete latex document should be created:
if input.makeCompleteLatexDocument
    latexHeader = {'\documentclass[a4paper,10pt]{article}';'\begin{document}'};
    latexFooter = {'\end{document}'};
    latex = [latexHeader;latex;latexFooter];
end
% print easy to copy&paste latex code to console:
disp(char(latex));
