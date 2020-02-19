x = -1:0.01:1;
a = 20;
y = sqrt(2*a) * x.* exp(-a * x.^2 - 0.5);  

set(0,'defaulttextinterpreter','latex')
set(groot,'DefaultAxesTickLabelInterpreter','latex');

plot(x, y, 'k-', 'LineWidth', 1);
grid on;

set(gcf,'position',[0,0,400, 200])
set(gca,'color','white')
set(0,'DefaultAxesColor','none')
xlabel('$v_{rel}$', 'fontsize',12);
ylabel('$\phi(v_{rel})$', 'fontsize',12);
% saveas(gcf, 'bowplot', 'epsc'); 