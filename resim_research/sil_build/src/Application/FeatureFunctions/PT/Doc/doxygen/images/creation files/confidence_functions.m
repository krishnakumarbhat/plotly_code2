%metrics
dist_obj_to_border = [0 1 3 5];
dist_border_to_isect = [0 1 2 5];
dist_obj_to_path = [0.0 1.2 1.7 2.3 3.5];
similarity_trail_path = [0.0 0.5 1.2 1.8 2.3];

%confidence values
dist_obj_to_border_conf = [1.0 0.9 0.7 0.25];
dist_border_to_isect_conf = [1.0 0.85 0.75 0.5];
dist_obj_to_path_conf = [1.0 0.8 0.68 0.4 0.1];   
similarity_trail_path_conf = [1.0 0.8 0.56 0.3 0.1];


%plot dist_obj_to_border_metric
folder = 'C:/Plastic/';
fontsize = 22;
linewidth = 3;
f1=figure(1);
plot(dist_obj_to_border,dist_obj_to_border_conf, 'LineWidth',linewidth);
grid on
xlabel('$d_{\mathrm{idx,tp}}$','FontSize', fontsize,'Interpreter','latex');
ylabel('$c_{\mathrm{idx,tp}}$','FontSize', fontsize,'Interpreter','latex');
print(f1,'-dpdf','-r1200',[folder 'dist_obj_to_border.pdf']);
 
f2=figure(2);
plot(dist_border_to_isect,dist_border_to_isect_conf, 'LineWidth',linewidth);
grid on
xlabel('$d_{\mathrm{idx,hp}}$','FontSize', fontsize,'Interpreter','latex');
ylabel('$c_{\mathrm{idx,hp}}$','FontSize', fontsize,'Interpreter','latex');
print(f2,'-dpdf','-r1200',[folder 'dist_border_to_isect.pdf']);

f3=figure(3);
plot(dist_obj_to_path,dist_obj_to_path_conf, 'LineWidth',linewidth);
grid on
xlabel('$d_{\mathrm{comp,op}}$','FontSize', fontsize,'Interpreter','latex');
ylabel('$c_{\mathrm{comp,op}}$','FontSize', fontsize,'Interpreter','latex');
print(f3,'-dpdf','-r1200',[folder 'dist_obj_to_path.pdf']);


f4=figure(4);
plot(similarity_trail_path, similarity_trail_path_conf, 'LineWidth',linewidth);
grid on
xlabel('$d_{\mathrm{diff}}$','FontSize', fontsize,'Interpreter','latex');
ylabel('$c_{\mathrm{diff}}$','FontSize', fontsize,'Interpreter','latex');
print(f4,'-dpdf','-r1200',[folder 'similarity_trail_path.pdf']);

