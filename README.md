# Cyber-Brick
Proyecto academico

GDD

1.	Visión General del Proyecto

  •	Título del juego: Cyber Brick
  •	Género: Arcade 2D / Destrucción de bloques de neón.
  •	Plataforma: PC (Windows / Linux / macOS).
  •	Tecnologías: C++17/20, OpenGL 3.3+ (Core Profile), GLFW, GLM, GLAD, stb_image.h, Dear ImGui.
  •	Público Objetivo: Jugadores retro, fans del género arcade y apasionados por los visuales estilo Cyberpunk/Synthwave.
  •	Pitch: "Un Breakout cibernético de ritmo rápido donde destruyes bloques de neón dinámicos utilizando una pala cargada de energía y     efectos de luz programados en shaders."


2.	Mecánicas de Juego (Gameplay)

	Mecánicas Principales (Core Loop)
    •	Control de la Pala (Paddle):
      o	Movimiento horizontal en el eje X guiado por el ratón o teclado (teclas A/D o flechas).
      o	Tamaño y velocidad ajustables en tiempo real para calibrar la dificultad.
  •	Rebote de la Pelota y Físicas:
    o	La pelota se desplaza en el plano 2D calculando el ángulo de rebote según el punto exacto de impacto con la pala (golpear los   extremos   le da un ángulo más pronunciado).
    o	Detección de colisiones 2D tipo AABB vs. Círculo (Axis-Aligned Bounding Box vs. Ball Radius).
  •	Tipos de Bloques (Cyber Bricks):
    o	Bloque Estándar: Se destruye de 1 golpe (suma puntos).
    o	Bloque Blindado: Requiere 2 o 3 impactos; cambia de color (o intensidad de neón) a medida que recibe daño.
    o	Bloque Explosivo: Al destruirse, libera un pulso que destruye o daña los bloques adyacentes.
    o	Bloque Indestructible: Sirve como obstáculo permanente para desviar la trayectoria.
  •	Condiciones de Victoria y Derrota:
    o	Victoria: Destruir todos los bloques destructibles de la pantalla.
    o	Derrota: Perder todas las vidas (la pelota cae por debajo del borde inferior de la pantalla).

	Mecánicas Secundarias y Power-Ups
  •	Caída de Power-Ups: Al destruir ciertos bloques, caen ítems flotantes con efectos temporales:
    o	Laser Paddle: Permite disparar pequeños proyectiles desde los extremos de la pala.
    o	Multi-Ball: Clona la pelota actual en 3 pelotas independientes.
    o	Enlarge: Duplica el ancho de la pala.


4.	Arquitectura Técnica y OpenGL (Tech Spec)

	Pipeline Gráfico y Shaders
    •	Vertex Shader:
      o	Recibe los vértices de un cuadrilátero base (Quad) y los transforma con matrices MVP (Model, View, Projection) usando la librería GLM en modo de proyección ortográfica.
    •	Fragment Shader (Efecto Neon / Bloom Simulado):
      o	Brillo de Neón: Aplica un algoritmo de brillo en las orillas de la pala, la pelota y los bloques utilizando funciones de atenuación en el shader.
      o	Color Dinámico: Parámetro uniform vec3 spriteColor para cambiar instantáneamente el tono del bloque o los flashes de impacto.
    •	Sprite Renderer:
      o	Implementación de una clase SpriteRenderer que dibuja geometrías utilizando un solo VAO y VBO reutilizable para todos los quads (pala, pelota, bloques).
      o	Manejo del canal Alfa (Alpha Blending) para bordes suaves y transparencias con glEnable(GL_BLEND).

	Módulos del Motor en C++
    •	Game: Administra el estado global (GAME_MENU, GAME_ACTIVE, GAME_WIN), puntuación, niveles y vidas.
    •	ResourceManager: Carga y almacena de forma estática los Shaders y las Texturas comprimidas/PNG.
    •	SpriteRenderer: Encargado del renderizado 2D y el paso de matrices MVP al pipeline de OpenGL.
    •	GameObject y BallObject: Estructuras que representan entidades en pantalla (posicón, tamaño, velocidad, color, textura).
    •	PostProcessor (Opcional): Permite añadir efectos retro globales como Scanlines (líneas de monitor CRT) o pequeñas sacudidas de pantalla (Screen Shake) mediante un Frame Buffer Object (FBO).
    •	GUI / Debug (Dear ImGui): Ventana flotante de diagnóstico para:
      o	Ajustar en vivo la velocidad de la pelota y la pala.
      o	Cambiar el color del brillo neón de los bloques.
      o	Ver el contador de FPS y memoria consumida.


5.	Estilo Visual y Sonoro

	Estilo Gráfico:
    •	Paleta de colores Synthwave/Cyberpunk (Tonos magenta #FF007F, cian #00F0FF, púrpura #120024 y amarillo neón).
    •	Fondo con un emparrillado/malla en perspectiva (Retro Grid) animado ligeramente mediante un shader.

	Efectos Visuales (VFX):
    •	Sistema simple de partículas en C++: La pelota deja una estela brillante a su paso y los bloques destruidos liberan pequeños chispazos/píxeles de neón.

6.	Progreso y Diseño de Niveles

•	Nivel 1 – Core Initialization:
  o	Disposición tradicional de bloques estandarizados para que el jugador se familiarice con la física y la velocidad.
•	Nivel 2 – Cyber Grid:
  o	Introducción de bloques blindados e indestructibles en el centro creando canales de rebote.
•	Nivel 3 – Overload:
  o	Mezcla de bloques explosivos y un patrón dinámico donde la velocidad de la pelota aumenta progresivamente.
