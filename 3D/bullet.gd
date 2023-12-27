class_name Bullet extends GenericEntity

const SPEED: float = PI/2
var is_active: bool = false

# -----------------------------------------------

func _ready():
	entity_has_gravity = false

func _process(_delta):
	look_at(Vector3(0, get_position().y, 0))

func _physics_process(delta):
	velocity.y = 0
	super._physics_process(delta)

func set_alpha(alpha: float):
	entity_alpha = alpha

func set_direction(direction: EntityDirection):
	entity_direction = direction

func set_radius(radius: float):
	entity_radius = radius

# -----------------------------------------------

func entity_get_xz(_alpha: float) -> Vector2:
	return Vector2(entity_radius*sin(entity_alpha), entity_radius*cos(entity_alpha))

# @override
# Retorna si la entidad debe saltar
func entity_should_jump() -> bool:
	return false

# @override
# Retorna la velocidad de salto de la entidad
func entity_get_jump_velocity() -> float:
	return 0

# @override
# Retorna el siguiente alpha, en base al alpha
# actual, la dirección, y el delta
func entity_get_new_alpha(current_alpha: float, direction: EntityDirection, delta: float) -> float:
		return current_alpha + direction * SPEED * delta

# @override
# Retorna la siguiente dirección
func entity_get_new_direction(current_direction: EntityDirection) -> EntityDirection:
	return current_direction
