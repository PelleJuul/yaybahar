x = 0:0.01:1;
d1 = zeros(1, length(x));
d2 = zeros(1, length(x));
d4 = zeros(1, length(x));
h = 0.001;

f = @(x) 10 * x^3 + 2*x^2;
f2 = @(x) (1 / h^2) * (f(x-h) - 2 * f(x) + f(x+h));
f4 = @(x) (1 / h^2) * (f2(x-h) - 2 * f2(x) + f2(x+h)); 

for i = 1:length(x)
   d1(i) = f(x(i));
   d2(i) = 0.01 * f2(x(i));
   d4(i) = 0.001 * f4(x(i));
end

set(0,'defaulttextinterpreter','latex')
set(groot,'DefaultAxesTickLabelInterpreter','Tex');

plot(x, d1, 'k-', 'LineWidth', 1);
grid on;
hold on;
plot(x, d2, 'k--', 'LineWidth', 1);
plot(x, d4, 'k:', 'LineWidth', 1);
legend('u', '10^{-2} u_{xx}', '10^{-3} u_{xxxx}', 'Location', 'southoutside', 'Orientation','horizontal');
hold off;

% set(gcf,'position',[0,0,400, 200])
% set(gca,'color','white')
% set(0,'DefaultAxesColor','none')
% xlabel('x (position along string)', 'fontsize',16);
% ylabel('y (displacement)', 'fontsize',16);
% saveas(gcf, 'diffplot', 'epsc');
