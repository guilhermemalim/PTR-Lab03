mat = csvread("out.txt"); 

t = mat(:,1);

ref_x = mat(:, 2);
ref_y = mat(:, 3);


Ymponto_x = mat(:, 4);
Ymponto_y = mat(:, 5);

Ym_x = mat(:, 6);
Ym_y = mat(:, 7);

V_x = mat(:, 8);
V_y = mat(:, 9);

U_x = mat(:, 10);
U_y = mat(:, 11);

X_x = mat(:, 12);
X_y = mat(:, 13);
X_w = mat(:, 14);

Y_x = mat(:, 15);
Y_y = mat(:, 16);

plot(ref_x, ref_y);
title('ref_x, ref_y');

figure;
plot(X_x, t);
title('Xc, t');

figure;
plot(X_y, t);
title('Yc, t');

figure;
plot(X_w, t);
title('Wc, t');

figure;
plot(X_x, X_y);
title('Xc, Yc');

%%
mat = csvread("out.txt"); 
x = mat(:,1);
y2 = mat(:,6);
y1= mat(:,7);
plot(y2,y1);
title('Grafico de (Xc(t),yc(t)) - Taxa de amostragem: 0.5');
legend('','','') ;



