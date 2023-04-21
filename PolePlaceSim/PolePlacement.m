pkg load signal
pkg load control
clear all

%VALORES REALES DEL SISTEMA PLANTA, MEDIDOS CON TESTER
R1 = 9.970e3
C1 = 10.29e-6
R2 = 974.6
C2 = 9.49e-6
h = 0.001

%ECUACION DEL SISTEMA RC
A = [ (-1/C1)*(1/R1 + 1/R2) 1/(R2*C1);1/(C2*R2) -1/(R2*C2)]
B = [1/(R1*C1) ; 0]
C = [0 1]
D = 0

%SE OBTIENE LA FUNCION DE TRANSFERENCIA EN TIEMPO CONTINUO A PARTIR DE MODELO ESTADOS
[num,den] = ss2tf(A,B,C,D);
Hs = tf(num,den)

%SE OBTIENE LA FUNCION DE TRANSFERENCIA EN TIEMPO DISCRETO
Hz = c2d(Hs, h, 'zoh')
[numz, denz] = tfdata(Hz, 'v')

%
Phi = expm(A*h)
Gamma = (expm(A*h) - expm(A*0))/A*B
C_z = C
D_z = D

% SE OBTIENE LA RESPUESTA AL ESCALON EN TIEMPO CONTINUO/DISCRETO, JUNTO CON LOS POLOS Y CEROS

figure;
step(Hs, Hz)

%
poles_LA =pole(Hz)

%
zplane(numz, denz)

% CALCULO DE K PARA MEJORAR LA RESPUESTA DEL SISTEMA 
poles = [0.5; 0.01]
K = place(Phi, Gamma, poles)

%
Phi_LC = Phi - Gamma*K
K0 = (C * (eye(2) - Phi_LC)^(-1) * Gamma)^(-1)
Gamma_LC = Gamma * K0

%
t = h:h:40;
r = [ zeros(length(t)/2, 1); ones(length(t)/2, 1) ];

%
y_open = filter(numz, denz, r);
[numz_pole_placement, denz_pole_placement] = ss2tf(Phi_LC, Gamma_LC, C_z, D_z);
y_pole_placement = filter(numz_pole_placement, denz_pole_placement, r);

%
figure;
hold on;
stairs(t, r)
stairs(t, y_open, "LineWidth", 3)
stairs(t, y_pole_placement, "LineWidth", 3)
legend("ENTRADA", "RESPUESTA NATURAL PLANTA", "SALIDA CONTROLADA")