%{/*
 Copyright © 2010-2016 by FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */
}%

function [engine] = compare(fisFile, fldFile, delimiter, hasMetadata)
if (nargin < 3)
    delimiter = ' ';
end
if (nargin < 4)
    hasMetadata = true;
end

engine = readfis(fisFile);
flMatrix = dlmread(fldFile, delimiter, hasMetadata ~ = 0, 0);

if (length(engine.input) + length(engine.output) ~ = size(flMatrix, 2))
    error('fuzzylite:compare.m', 'Number of inputs and outputs in engine differ from FLD matrix');
end

if (isempty(engine.andMethod))
    engine.andMethod = 'min';
end
if (isempty(engine.orMethod))
    engine.orMethod = 'max';
end

engine.inputValues = flMatrix(1 : end, 1 : length(engine.input));
engine.outputValues = evalfis(engine.inputValues, engine);
engine.flOutputValues = flMatrix(1 : end, (length(engine.input) + 1) : (length(engine.input) + length(engine.output)));
engine.outputDiff = engine.outputValues - engine.flOutputValues;
engine.fld = [engine.inputValues engine.outputValues engine.flOutputValues engine.outputDiff];
engine.nanfreeDiff = engine.outputDiff;
engine.nanfreeDiff(find(isnan(engine.nanfreeDiff))) = 0;
engine.mse = nansum(engine.outputDiff.^2) / size(engine.outputDiff, 1);
engine.quantiles = prctile(engine.nanfreeDiff, 0 : 25 : 100);

end



