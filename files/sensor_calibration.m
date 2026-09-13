% Code written to calibrate ACS772 current sensor + level shifter against a
% normal current sensor.
% Author : Harikrishnan S
% Date   : 13-11-2025


clc; clear; close all;

dataMatrix = readmatrix('./Current_sensor_2/test_4.csv'); 
dataMatrix(1:2,:) = [];

actual     = dataMatrix(:, 2); 
sensor     = dataMatrix(:, 3);



% Assume sensor and actual are already loaded with 2000 data points each
num_points = 2000;

x = sensor; % Independent variable
y = actual; % Dependent variable

slopex = [];
interceptx = [];

% Parameters
chunk_size = 1000;  
num_chunks = num_points / chunk_size;  % 2000 / 100 = 20 chunks

% Initialize storage for slopes (m) and intercepts (c)
slopes = zeros(1, num_chunks);
intercepts = zeros(1, num_chunks);

% Subplot tracking
subplot_index = 1;  % Tracks the subplot position (1 to 4 per figure)
figure_count = 1;   % Tracks total figures

for i = 1:num_chunks
    % Extract the current chunk of 100 data points
    idx_start = (i-1) * chunk_size + 1;
    idx_end = idx_start + chunk_size - 1;
    
    x_chunk = x(idx_start:idx_end);
    y_chunk = y(idx_start:idx_end);
    
    % Compute best fit line (Linear Regression)
    coeffs = polyfit(x_chunk, y_chunk, 1);
    
    % Store slope and intercept
    slopes(i) = coeffs(1); 
    intercepts(i) = coeffs(2);

    slopex(end+1) = slopes(i);
    interceptx(end+1) = intercepts(i);

    % Generate best fit line data
    x_fit = linspace(min(x_chunk), max(x_chunk), 100);
    y_fit = polyval(coeffs, x_fit);

    % Create new figure after every 4 subplots
    if subplot_index == 1
        figure(figure_count); % Open a new figure for the next 4 graphs
        figure_count = figure_count + 1;
    end

    % Create subplot (2x2 grid)
    subplot(1,2,subplot_index);
    scatter(x_chunk, y_chunk, 'ro', 'filled'); % Scatter plot of original data
    hold on;
    plot(x_fit, y_fit, 'b-', 'LineWidth', 2); % Best fit line
    grid on;

    % Labels and Title
    xlabel('Sensor Data');
    ylabel('Actual Data');
    title(sprintf('Chunk %d: y = %.4fx + %.4f', i, slopes(i), intercepts(i)));
    legend('Original Data', 'Best Fit Line');

    % Increment subplot index
    subplot_index = subplot_index + 1;

    % Reset subplot index after every 4 plots (new figure starts)
    if subplot_index > 2
        subplot_index = 1;
    end
end

% Compute and display mean slope and intercept
mean_slope = mean(slopex) ;
mean_intercept = mean(interceptx) ;
fprintf('\nMean Slope: %.4f\n', mean_slope);
fprintf('Mean Intercept : %.4f\n', mean_intercept);


i_actual     = actual;       % actual current
i_calibrated = mean(slopex)*sensor+mean(interceptx);   % calibrated current (same shape)
t = (0:1e-4:(length(actual)-1)*1e-4 )';

% Create figure
figure('Color','w');
hold on; grid on; box on;

% Plot actual current
p1 = plot(t, i_actual, 'LineWidth', 1.8, 'Color', [0 0.45 0.74]); % blue

% Plot calibrated current 
p2 = plot(t, i_calibrated, '--', 'LineWidth', 1.8, 'Color', [0.85 0.33 0.10]); % orange dashed


% Titles and labels
title('Actual vs Calibrated Current','FontWeight','bold','FontSize',14);
xlabel('Time (s)','FontSize',12);
ylabel('Current (A)','FontSize',12);

% Legend
legend([p1 p2], {'Actual Current','Calibrated Current'}, ...
       'Location','best','FontSize',11);

% Improve layout
xlim([t(1) t(end)]);
set(gca,'FontName','Times New Roman','FontSize',11);

hold off;




