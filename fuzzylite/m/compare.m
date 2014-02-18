
function [engine] = compare(fisFile, fldFile, delimiter, hasMetadata)
    if (nargin < 3)
        delimiter = ' ';
    end
    if (nargin < 4)
        hasMetadata = true;
    end

    engine = readfis(fisFile);
    flMatrix = dlmread(fldFile, delimiter, hasMetadata ~= 0, 0);

    if (length(engine.input) + length(engine.output) ~= ndims(flMatrix))
        error('fuzzylite.m', 'Number of inputs and outputs in engine differ from FLD matrix');
    end

    if (isempty(engine.andMethod))
        engine.andMethod = 'min';
    end
    if (isempty(engine.orMethod))
        engine.orMethod = 'max';
    end

    engine.inputValues = flMatrix(1:end, 1:length(engine.input));
    engine.outputValues = evalfis(engine.inputValues, engine);
    engine.flOutputValues = flMatrix(1:end, (length(engine.input) + 1):(length(engine.input) + length(engine.output)));
    engine.outputDiff=engine.outputValues-engine.flOutputValues;
    engine.mse=nansum(engine.outputDiff.^2)/size(engine.outputDiff,1);
    %TODO: five number summary
end



