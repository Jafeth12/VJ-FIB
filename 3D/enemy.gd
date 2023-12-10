class_name GenericEnemy extends CharacterBody3D

# Estado del enemigo
enum EnemyState { WAIT, WANDER, ATTACK, DEAD }
var enemy_state: EnemyState = EnemyState.WAIT

# Movimiento circular
@export var INIT_ALPHA: float = 0
var alpha: float = INIT_ALPHA
var old_alpha: float = alpha
var radius = 18

const SPEED = PI/10
# const JUMP_VELOCITY = 4.5

# Get the gravity from the project settings to be synced with RigidBody nodes.
# var gravity = ProjectSettings.get_setting("physics/3d/default_gravity")
const gravity = 10

func _process(_delta):
	next_state()
	act_uppon_state()
	update_animation()
	if enemy_state == EnemyState.WAIT:
		return

func _physics_process(delta):
	# Add the gravity.
	if not is_on_floor():
		velocity.y -= gravity * delta
	
	# Get the input direction and handle the movement/deceleration.
	# As good practice, you should replace UI actions with custom gameplay actions.
	
	# Calculate new position, based on alpha
	alpha = get_new_alpha(alpha, delta)
	var next_xz = get_next_xz(alpha)

	# Calculate velocity to get to the new position
	velocity.x = (next_xz.x - get_position().x)/delta
	velocity.z = (next_xz.y - get_position().z)/delta
	
	# Correct current velocity
	var length: float = velocity.length()
	if length < 0.005:
		velocity = Vector3(0, 0, 0)
	
	move_and_slide()
	if is_on_wall():
		alpha = get_position_alpha(get_position())
	old_alpha = alpha


# Funciones reimplementables segun el enemigo
func next_state():
	var prx_state: EnemyState = enemy_state
	match enemy_state:
		EnemyState.WAIT:
			if should_activate():
				prx_state = EnemyState.WANDER
		EnemyState.WANDER:
			if should_die():
				prx_state = EnemyState.DEAD
			elif should_attack():
				prx_state = EnemyState.ATTACK
			else:
				prx_state = EnemyState.WANDER
		EnemyState.ATTACK:
			if should_die():
				prx_state = EnemyState.DEAD
			elif is_attack_finished():
				prx_state = EnemyState.WANDER
			else:
				prx_state = EnemyState.ATTACK
	enemy_state = prx_state


func get_new_alpha(current_alpha: float, delta: float) -> float:
	return current_alpha

# Funciones a reimplementar en cada enemigo
func act_uppon_state() -> void:
	pass

func update_animation() -> void:
	pass

# Return if the player should activate when it is in WAIT state
# p.e.: When the player is on the same platform as you
func should_activate() -> bool:
	return true

# Use this function to see if the enemy should attack the player
# p.e.: if the player is close enough
func should_attack() -> bool:
	return false

func should_die() -> bool:
	return false

func should_revive() -> bool:
	return false

func is_attack_finished() -> bool:
	return false

# Funciones a reimplementar cuando se abstraiga el nodo entidad
func get_next_xz(_alpha: float) -> Vector2:
	return Vector2(radius*sin(_alpha), radius*cos(_alpha))

# Devuelve el ángulo alpha en base a la posición del jugador
func get_position_alpha(_pos: Vector3) -> float:
	return old_alpha # TODO
