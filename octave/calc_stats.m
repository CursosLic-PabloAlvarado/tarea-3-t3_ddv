% Script: calc_stats.m

% leyendo el archivo de stats
filename = 'stats.txt';
data = dlmread(filename);

% calculando el promedio
promedio = mean(data);

% calculando la desviación estándar
desviacion_estandar = std(data);

% calculando la varianza
varianza = var(data);

% obtiene la fecha y hora actual
fecha_hora = strftime('%Y-%m-%d %H:%M:%S', localtime(time()));

% guardar historial
historial = 'historial.txt';
f = fopen(historial, 'a');  % Abre el archivo en modo "append"
fprintf(f, 'Fecha y hora: %s\n', fecha_hora);
fprintf(f, 'Promedio: %.2f ns\n', promedio);
fprintf(f, 'Desviación estándar: %.2f ns\n', desviacion_estandar);
fprintf(f, 'Varianza: %.2f ns^2\n', varianza);
fprintf(f, '---\n');  % Separador para futuras ejecuciones
fclose(f);  % Cierra el archivo
