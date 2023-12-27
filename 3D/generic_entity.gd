class_name GenericEntity extends CharacterBody3D

enum EntityDirection { LEFT = -1, NONE = 0, RIGHT = 1 }

var entity_alpha: float = 0
var entity_direction: EntityDirection = EntityDirection.LEFT
var entity_has_gravity: bool = true
@export var entity_radius: float = 18

# FIXME
var old_alpha = entity_alpha

# Get the gravity from the project settings to be synced with RigidBody nodes.
var gravity = ProjectSettings.get_setting("physics/3d/default_gravity")

# Loop principal de las físicas de una entidad
func _physics_process(delta):
	# Add the gravity.
	if entity_has_gravity:
		if !is_on_floor():
			velocity.y -= gravity * delta

	# Handle jump.
	if entity_should_jump():
		velocity.y = entity_jump()

	# Calculate new position, based on alpha
	entity_direction = entity_get_new_direction(entity_direction)
	entity_alpha = entity_get_new_alpha(entity_alpha, entity_direction, delta)
	var next_xz = entity_get_xz(entity_alpha)

	# Calculate velocity to get to the new position
	velocity.x = (next_xz.x - get_position().x)/delta
	velocity.z = (next_xz.y - get_position().z)/delta

	# Correct current velocity
	var length: float = velocity.length()
	if length < 0.005:
		velocity = Vector3(0, 0, 0)

	# Move
	move_and_slide()

	# Correct alpha
	if is_on_wall():
		entity_alpha = entity_get_position_alpha(get_position())
	old_alpha = entity_alpha

# Retorna la posición en base al alpha
func entity_get_xz(alpha: float) -> Vector2:
	return Vector2(entity_radius*sin(alpha), entity_radius*cos(alpha))

# Retorna la el alpha en base a la posición
# FIXME: esto solo es un arreglo temporal.
#        Funciona, pero mal
func entity_get_position_alpha(_pos: Vector3):
	return old_alpha

# VIRTUAL. TO BE OVERRIDEN
# Retorna si la entidad debe saltar
func entity_should_jump() -> bool:
	return false

# VIRTUAL. TO BE OVERRIDEN
# Retorna la velocidad de salto de la entidad
# SIDE EFFECTS: toca las variables que sean necesarias de lógica en la implementación
func entity_jump() -> float:
	return 0

# VIRTUAL. TO BE OVERRIDEN
# Retorna el siguiente alpha, en base al alpha
# actual, la dirección, y el delta
func entity_get_new_alpha(current_alpha: float, direction: EntityDirection, delta: float) -> float:
	return current_alpha

# VIRTUAL. TO BE OVERRIDEN
# Retorna la siguiente dirección
func entity_get_new_direction(current_direction: EntityDirection) -> EntityDirection:
	return EntityDirection.LEFT
