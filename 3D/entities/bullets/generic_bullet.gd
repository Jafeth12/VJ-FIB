class_name GenericBullet extends GenericEntity

#@export var mesh: Mesh = null
@export var mesh_scale: float = 1
@export var light_color: Color = Color("black")

@export var BULLET_SPEED: float = PI/1.5
@export var BULLET_MAX_TRAVEL_DISTANCE: float = PI/2
@export var BULLET_DAMAGE: int = 33

var BULLET_INITIAL_ALPHA: float = 0

# -----------------------------------------------

func _ready():
	entity_has_gravity = false
	$bullet_activation_area.connect("body_entered", bullet_body_entered)
	#$mesh.mesh = mesh
	$mesh.scale = Vector3(mesh_scale, mesh_scale, mesh_scale)
	$light.light_color = light_color

func init(pos: Vector3, alpha: float, direction: EntityDirection, radius: float, _playerCrouching: bool):
	BULLET_INITIAL_ALPHA = alpha
	set_position(pos)
	bullet_set_alpha(alpha)
	bullet_set_direction(direction)
	bullet_set_radius(radius)

func _process(_delta):
	look_at(Vector3(0, get_position().y, 0))

func _physics_process(delta):
	velocity.y = 0
	super._physics_process(delta)

	if abs(entity_alpha - BULLET_INITIAL_ALPHA) > BULLET_MAX_TRAVEL_DISTANCE:
		queue_free()

func bullet_set_alpha(alpha: float):
	entity_alpha = alpha

func bullet_set_direction(direction: EntityDirection):
	entity_direction = direction
	if direction == EntityDirection.LEFT:
		$mesh.rotate_y(PI)
	

func bullet_set_radius(radius: float):
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

# VIRTUAL FUNCTION -> TO BE OVERRIDEN
func bullet_body_entered(body: Node3D) -> void:
	if body is GenericEnemy:
		# Bullet hit enemy
		if body.enemy_is_dead():
			return
		body.enemy_take_damage(BULLET_DAMAGE)

	elif body is StaticBody3D:
		# Bullet hit wall
		pass

	else:
		# player is here and other stuff is here
		return

	queue_free()

