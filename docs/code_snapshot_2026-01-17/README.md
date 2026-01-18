# Asistente Senior Trosky

Sistema IoT de asistencia inteligente basado en ESP32-S3, diseñado para proporcionar monitoreo ambiental, asistencia por voz y notificaciones de emergencia.

## 📋 Descripción

Trosky es un sistema distribuido de dos nodos:
- **Nodo Baño**: Panel de seguridad con sensor ambiental, pantalla TFT y botón SOS
- **Nodo Sala**: Espejo inteligente con cámara y asistencia por voz

## 🏗️ Arquitectura

```
trosky/
├── include/
│   ├── config.h.template    # Plantilla de configuración
│   ├── HAL.h                 # Abstracción de hardware
│   ├── app/                  # Módulos de aplicación
│   ├── engine/               # Framework core
│   └── services/             # Servicios externos
├── src/
│   ├── main.cpp              # Punto de entrada
│   ├── app/                  # Implementación de módulos
│   ├── engine/               # Implementación del framework
│   └── services/             # Implementación de servicios
└── platformio.ini            # Configuración de compilación
```

### Principios de Diseño
- **Separación de Responsabilidades**: `engine/` (framework) vs `app/` (lógica específica)
- **Event-Driven**: Comunicación desacoplada vía `EventBus`
- **Dependency Injection**: Servicios registrados en `ServiceLocator`
- **Hardware Abstraction**: Todos los pines en `HAL.h`

## 🚀 Inicio Rápido

### Prerrequisitos
- [PlatformIO](https://platformio.org/) instalado
- ESP32-S3 con pantalla TFT (Nodo Baño) o ESP32-S3-CAM (Nodo Sala)
- Cuenta de WhatsApp y API Key de [CallMeBot](https://www.callmebot.com/)

### Configuración

1. **Clonar el repositorio**
   ```bash
   git clone <repository-url>
   cd trosky
   ```

2. **Configurar credenciales**
   ```bash
   cp include/config.h.template include/config.h
   # Editar include/config.h con tus credenciales
   ```

3. **Compilar y cargar**
   ```bash
   # Para Nodo Baño
   pio run -e bathroom_node -t upload
   
   # Para Nodo Sala
   pio run -e living_room_node -t upload
   ```

## ⚙️ Configuración

Edita `include/config.h` con tus valores:

```cpp
#define WIFI_SSID "TuRedWiFi"
#define WIFI_PASSWORD "TuContraseña"
#define WHATSAPP_PHONE "+1234567890"
#define WHATSAPP_API_KEY "tu_api_key"
#define TIMEZONE_OFFSET -18000  // GMT-5 (ajustar según tu zona)
```

## 📱 Características

### Nodo Baño
- ✅ Sensor BMP280 (temperatura y presión)
- ✅ Pantalla ST7789 (240x135) con UI animada
- ✅ Reloj sincronizado por SNTP
- ✅ Indicador día/noche (Sol/Luna)
- ✅ Botón SOS con notificación WhatsApp
- ✅ Animación "Trosky" durante escucha por voz

### Nodo Sala
- ✅ Cámara OV2640 para identificación de medicamentos
- ✅ Micrófono INMP441 para comandos de voz
- ✅ Detección de palabra clave "Trosky"
- ✅ Solicitud de taxi por voz
- ✅ Asistencia de recetas
- 🚧 Procesamiento IA (Mock implementado)

## 🔧 Desarrollo

### Estructura del Código

**Engine (Framework)**
- `AppState`: Estado global de la aplicación
- `EventBus`: Sistema de eventos pub/sub
- `ServiceLocator`: Registro de servicios
- `Tools`: Herramientas reutilizables (Sensor, Input, Notification)

**App (Lógica Específica)**
- `BathroomNode`: Orquestador del nodo baño
- `BathroomUI`: Interfaz gráfica del baño
- `LivingRoomNode`: Orquestador del nodo sala

**Services**
- `WhatsAppService`: Integración con CallMeBot
- `AIService`: Interfaz para procesamiento IA (Mock)

### Añadir un Nuevo Módulo

1. Crear header en `include/app/modules/tu_modulo/`
2. Crear implementación en `src/app/modules/tu_modulo/`
3. Registrar en `main.cpp` vía `ServiceLocator`
4. Suscribirse a eventos vía `EventBus`

### Compilar Solo un Nodo

```bash
# Nodo Baño
pio run -e bathroom_node

# Nodo Sala
pio run -e living_room_node
```

## 🐛 Troubleshooting

### Error: "config.h file not found"
**Solución**: Copia `include/config.h.template` a `include/config.h` y configura tus credenciales.

### Error: "WhatsAppService not registered"
**Solución**: Verifica que `main.cpp` registre el servicio antes de inicializar los nodos.

### Pantalla en blanco
**Solución**: 
1. Verifica los pines en `HAL.h` coincidan con tu hardware
2. Revisa que `TFT_eSPI` esté configurado correctamente en `platformio.ini`

### Sensor BMP280 no responde
**Solución**: 
1. Verifica la dirección I2C (típicamente `0x76` o `0x77`, no `0x6B`)
2. Usa un escáner I2C para detectar la dirección correcta

### Hora incorrecta
**Solución**: Ajusta `TIMEZONE_OFFSET` en `config.h` según tu zona horaria.

## 📊 Logs y Debugging

El sistema usa logging estructurado con niveles:
- `[DEBUG]`: Información de depuración
- `[INFO]`: Eventos normales
- `[WARN]`: Advertencias no críticas
- `[ERROR]`: Errores recuperables
- `[CRITICAL]`: Errores críticos del sistema

Monitor serial a 115200 baud:
```bash
pio device monitor -b 115200
```

## 🔐 Seguridad

- ✅ Credenciales externalizadas en `config.h` (no versionado)
- ✅ `.gitignore` configurado para proteger archivos sensibles
- ⚠️ CallMeBot API usa HTTP (no HTTPS) - solo para prototipos

## 📝 Licencia

[Especificar licencia]

## 🤝 Contribuciones

Las contribuciones son bienvenidas. Por favor:
1. Fork el proyecto
2. Crea una rama para tu feature (`git checkout -b feature/AmazingFeature`)
3. Commit tus cambios (`git commit -m 'Add some AmazingFeature'`)
4. Push a la rama (`git push origin feature/AmazingFeature`)
5. Abre un Pull Request

## 📞 Soporte

Para reportar bugs o solicitar features, abre un issue en GitHub.

---

**Versión**: 1.0.0  
**Última actualización**: 2026-01-17
