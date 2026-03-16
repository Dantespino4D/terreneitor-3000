# Sintaxis para .md
## Division de secciones    
1. #Titulo
2. ##Subtitulo
3. ###Seccion
4. ####Subseccion

Nota: Siempre escribir un espacio entre los "#" y la seccio
## Negritas y Cursivas
- Esto es texto normal

- Esto es texto en **negritas**

- Esto es texto en *cursiva*

- Esto es texto en ***negritas y cursiva***

- ~~Este texto está tachado~~
## Listas
- Ejemplo 3
- Ejemplo 4

    - Ejemplo 5
    - Ejemplo 6

1. Ejemplo 7
2. Ejemplo 8
## Escribir Codigo

Esta prohibida la funcion `delay();` en este proyecto.

Para bloques de código de varias líneas: Usa tres acentos graves (```). Si pones el nombre del lenguaje justo después, GitHub y VSC le pondrán los colores correctos.

Aquí está el código de mi sensor:
```cpp
void loop() {
  int valor = analogRead(A0);
  Serial.println(valor);
}
```
## Enlaces (Links) e Imágenes
[Enlace a mi GitHub: NODO](https://github.com/aizenWN/NODO_Plataforma_Cognitiva)

![Logo de Nodo](docs/Logo_NODO_prueba.png)
## Citas
> **Nota importante del 14 de Febrero:**
> El pin A4 y A5 se usan obligatoriamente para la pantalla OLED (I2C).
## Lista de Tareas
- [x] Crear el repositorio en GitHub.
- [x] Subir la versión 0.1.
- [ ] Programar la lógica del Estrés (v0.2).
- [ ] Integrar animación en los ojos.