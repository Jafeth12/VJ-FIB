class_name GenericEnemy extends CharacterBody3D

# Estado del enemigo
enum EnemyState { WAIT, WANDER, ATTACK, DEAD }
enum EnemyDirection { LEFT = -1, RIGHT = 1}
var enemy_state: EnemyState = EnemyState.WAIT
var enemy_dir: EnemyDirection  = EnemyDirection.LEFT

# Movimiento circular
@export var INIT_ALPHA: float = 0
var entity_alpha: float = INIT_ALPHA
var old_alpha: float = entity_alpha
var entity_radius = 18

const SPEED = PI/10
# const JUMP_VELOCITY = 4.5

# Get the gravity from the project settings to be synced with RigidBody nodes.
# var gravity = ProjectSettings.get_setting("physics/3d/default_gravity")
const gravity = 10

func _process(_delta):
	enemy_update_state()
	enemy_update_animation()

func _physics_process(delta):
	# Add the gravity.
	if not is_on_floor():
		velocity.y -= gravity * delta
	
	# Get the input direction and handle the movement/deceleration.
	# As good practice, you should replace UI actions with custom gameplay actions.
	
	# Calculate new position, based on alpha
	entity_alpha = entity_get_new_alpha(entity_alpha, delta)
	var next_xz = entity_get_next_xz(entity_alpha)

	# Calculate velocity to get to the new position
	velocity.x = (next_xz.x - get_position().x)/delta
	velocity.z = (next_xz.y - get_position().z)/delta
	
	# Correct current velocity
	var length: float = velocity.length()
	if length < 0.005:
		velocity = Vector3(0, 0, 0)
	
	move_and_slide()
	if is_on_wall():
		entity_alpha = entity_get_position_alpha(get_position())
	old_alpha = entity_alpha


# Funciones reimplementables segun el enemigo
func enemy_update_state():
	var prx_state: EnemyState = enemy_state
	match enemy_state:
		EnemyState.WAIT:
			if enemy_should_activate():
				prx_state = EnemyState.WANDER
		EnemyState.WANDER:
			if enemy_should_die():
				prx_state = EnemyState.DEAD
			elif enemy_should_attack():
				prx_state = EnemyState.ATTACK
			else:
				prx_state = EnemyState.WANDER
		EnemyState.ATTACK:
			if enemy_should_die():
				prx_state = EnemyState.DEAD
			elif enemy_is_attack_finished():
				prx_state = EnemyState.WANDER
			else:
				prx_state = EnemyState.ATTACK
	enemy_state = prx_state

func enemy_update_animation() -> void:
	pass

func entity_get_new_alpha(current_alpha: float, _delta: float) -> float:
	return current_alpha

# Return if the player should activate when it is in WAIT state
# p.e.: When the player is on the same platform as you
func enemy_should_activate() -> bool:
	return true

# Use this function to see if the enemy should attack the player
# p.e.: if the player is close enough
func enemy_should_attack() -> bool:
	return false

func enemy_should_die() -> bool:
	return false

func enemy_should_revive() -> bool:
	return false

func enemy_is_attack_finished() -> bool:
	return false

# Funciones a reimplementar cuando se abstraiga el nodo entidad
func entity_get_next_xz(alpha: float) -> Vector2:
	return Vector2(entity_radius*sin(alpha), entity_radius*cos(alpha))

# Devuelve el ángulo alpha en base a la posición del jugador
func entity_get_position_alpha(_pos: Vector3) -> float:
	return old_alpha # TODO
