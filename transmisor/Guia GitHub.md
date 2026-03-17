# 🚀 Guía Git Básica y Práctica (VS Code + Terminal)

## 🧠 Conceptos clave

- **Repositorio**: Proyecto con control de versiones
- **Commit**: Foto (snapshot) de tu proyecto
- **Branch (rama)**: Línea de trabajo independiente
- **main**: Rama principal (no tocar directamente)

---

## 🌿 Manejo de ramas

### Crear una rama
```bash
git checkout -b nombre-rama
```

### Cambiar de rama
```bash
git checkout nombre-rama
```

O moderno:
```bash
git switch nombre-rama
```

### Ver ramas
```bash
git branch
```

---

## 📦 Flujo básico de trabajo

```bash
git add .
git commit -m "mensaje"
git push origin nombre-rama
```

---

## 📁 Subir archivos específicos

### Archivo en raíz
```bash
git add archivo.cpp
```

### Archivo dentro de carpeta
```bash
git add carpeta/archivo.cpp
```

### Carpeta completa
```bash
git add carpeta/
```

---

## ⚠️ Carpetas vacías

Git NO sube carpetas vacías.

Solución:
```bash
touch carpeta/.gitkeep
```

---

## 🚫 Ignorar archivos (.gitignore)

Ejemplo:
```
.pio/
.vscode/
managed_components/
```

---

## 🧹 Eliminar archivos del repo pero no del disco

```bash
git rm -r --cached carpeta/
```

Luego:
```bash
git commit -m "Elimino carpeta del repo"
git push
```

---

## 🔄 Ver estado

```bash
git status
```

---

## 🔍 Ver archivos en Git

```bash
git ls-files
```

---

## 💣 Resetear todo (PELIGRO)

```bash
git fetch origin
git reset --hard origin/main
git clean -fd
```

---

## 🧽 Limpiar archivos no rastreados

```bash
git clean -fd
```

---

## 📥 Pull (descargar cambios)

```bash
git pull origin nombre-rama
```

---

## 📤 Push (subir cambios)

```bash
git push origin nombre-rama
```

---

## 🧠 Reglas importantes

- No trabajes directamente en `main`
- Usa ramas para pruebas
- Haz commits pequeños y claros
- Usa `.gitignore`
- Verifica con `git status` antes de subir

---

## 🔥 Comandos PRO (muy importantes)

### Guardar cambios temporalmente
```bash
git stash
```

### Recuperar cambios guardados
```bash
git stash pop
```

### Ver historial
```bash
git log
```

### Ver cambios exactos
```bash
git diff
```

### Agregar por partes (avanzado)
```bash
git add -p
```

---

## 🧠 Mentalidad clave

> Git no guarda archivos, guarda versiones del proyecto

> Solo necesitas subir los cambios, Git mantiene lo demás

---

## ✅ Flujo recomendado

```bash
git checkout -b mi-rama
git add .
git commit -m "cambios"
git push origin mi-rama
```

---

## 🎯 Objetivo final

Trabajar seguro sin romper `main`, usando ramas para experimentar.

---

Si entiendes esto, ya estás por encima del promedio en Git 🚀

