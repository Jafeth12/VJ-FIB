class_name Bullet extends GenericEntity

const BULLET_SPEED: float = PI/1.5
const BULLLET_MAX_TRAVEL_DISTANCE: float = PI/2

var bullet_initial_alpha: float = 0

# -----------------------------------------------

func _ready():
	entity_has_gravity = false
	$bullet_activation_area.connect("body_entered", bullet_body_entered)

func init(pos: Vector3, alpha: float, direction: EntityDirection, radius: float):
	bullet_initial_alpha = alpha
	set_position(pos)
	set_alpha(alpha)
	set_direction(direction)
	set_radius(radius)

func _process(_delta):
	look_at(Vector3(0, get_position().y, 0))

func _physics_process(delta):
	velocity.y = 0
	super._physics_process(delta)

	if abs(entity_alpha - bullet_initial_alpha) > BULLLET_MAX_TRAVEL_DISTANCE:
		queue_free()

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
	return current_alpha + direction * BULLET_SPEED * delta

# @override
# Retorna la siguiente dirección
func entity_get_new_direction(current_direction: EntityDirection) -> EntityDirection:
	return current_direction

# ------------------------------------------------------

func bullet_body_entered(body: Node3D) -> void:
	print("body entered in Bullet area")
	print(body.get_name())

	if body is GenericEnemy:
		# print("Bullet hit enemy")
		pass

	elif body is StaticBody3D:
		# print("Bullet hit wall")
		pass

	else:
		# player is here and other stuff is here
		return

	queue_free()

