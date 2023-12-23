class_name Player extends GenericEntity

# Enumerations
enum ANIMATION_STATES { IDLE, WALK, JUMP, CROUCH, DIE, DODGE }
enum FACING { LEFT=-1, RIGHT=1 }
enum RING { EXTERIOR, INTERIOR }
enum LEVEL { LOWER, MIDDLE, UPPER }
var LEVEL_HEIGHTS = [ 0, 20, 30 ]

# Máquinas de estados
var anim_state = ANIMATION_STATES.IDLE
var facing = FACING.RIGHT
var curr_ring = RING.EXTERIOR
var changing_ring : bool = false
var curr_level : LEVEL = LEVEL.LOWER
var target_level : LEVEL = curr_level
var resetting_alpha : bool = false

# Velocidades angulares del jugador
@export var SPEED = PI/8 # 1 lap = 16 secs.
@export var DODGE_SPEED = PI/2

# Radios de anillos y velocidad de cambio
@export var RING_SWITCH_SPEED = 0.125
const radius_exterior = 20
const radius_interior = 15

var player_radius = radius_exterior # TODO: abtraer esto

# Jump parameters
const INIT_JUMPS_LEFT = 2
var jumps_left: int = INIT_JUMPS_LEFT # Cuenta el numero de saltos que puede dar el jugador
const JUMP_VELOCITY = 7
const RING_SWITCH_JUMP_VELOCITY = JUMP_VELOCITY*1.5

# ======== Reimplementaciones de funciones de CharacterBody3D ========

func _ready() -> void:
	$sprite.set_scale($sprite.scale)
	$sprite.play("idle")
	$sprite.connect("animation_finished", player_on_animation_finished)

# Gestionar la lógica que no tiene que ver con la física del jugador
# p.e.: Vida y muerte, cambio de anillo, animaciones, etc.
func _process(_delta: float) -> void:
	if player_should_die():
		player_die()
	if !player_is_dead():
		player_update_facing()
	player_handle_input()
	look_at(Vector3(0, get_position().y, 0))
	player_update_anim_state()

# Gestionar todo aquello relacionado con el movimiento del jugador.
# p.e.: Movimiento angular, saltos, colisiones, etc.
func _physics_process(delta: float) -> void:
	# Check if player is dead. Return inmediately now
	if player_is_dead():
		return

	# Add the gravity.
	if !is_on_floor():
		velocity.y -= gravity * delta

	# Changing ring
	if changing_ring:
		player_switch_ring()

	# Changing level
	if curr_level != target_level:
		player_switch_level()

	# Handle jump.
	if is_on_floor():
		jumps_left = INIT_JUMPS_LEFT

	if !is_on_floor() && jumps_left == INIT_JUMPS_LEFT:
		jumps_left = 1

	if entity_should_jump():
		player_jump()

	# super(delta)

	# Get the input direction and handle the movement/deceleration.
	# As good practice, you should replace UI actions with custom gameplay actions.
	var right_pressed: bool = Input.is_action_pressed("move_right")
	var left_pressed: bool = Input.is_action_pressed("move_left")

	# Get direction of movement
	var circular_dir: int = 0;
	if resetting_alpha:
		if entity_alpha > 0:
			circular_dir = -1
		else:
			circular_dir = 1
	elif (right_pressed && !left_pressed):
		circular_dir = 1
		facing = FACING.RIGHT
	elif left_pressed && !right_pressed:
		circular_dir = -1
		facing = FACING.LEFT

	# Update entity_alpha
	if resetting_alpha:
		var speed: float = SPEED*15
		var real_dir: int = circular_dir
		var next_alpha: float = entity_alpha + (real_dir * speed * delta)
		if (entity_alpha >= -0.05 && entity_alpha <= 0.05 ) || (next_alpha > 0 && entity_alpha < 0) or (next_alpha < 0 and entity_alpha > 0):
			entity_alpha = 0
			resetting_alpha = false
			player_change_level_state()
		else:
			entity_alpha = next_alpha
	else:
		if !player_is_crouching():
			var speed: float = DODGE_SPEED if player_is_dodging() else SPEED
			var real_dir: int = circular_dir if !player_is_dodging() else int(facing)
			entity_alpha += real_dir * speed * delta
			if entity_alpha > 2*PI:
				entity_alpha -= 2*PI
			elif entity_alpha < -2*PI:
				entity_alpha += 2*PI

	var next_xz = entity_get_xz(entity_alpha)
	velocity.x = (next_xz.x - get_position().x)/delta
	velocity.z = (next_xz.y - get_position().z)/delta

	var length: float = velocity.length()
	if length < 0.005:
		velocity = Vector3(0, 0, 0)

	move_and_slide()
	if is_on_wall():
		entity_alpha = old_alpha
		entity_alpha = entity_get_position_alpha(get_position())
	old_alpha = entity_alpha

# ======== Update maquinas de estados ========

# Actualizar la máquina de estados de las animacoines.
# Se activa la animación si hace falta (lógica de salida)
func player_update_anim_state():
	# prox_estado:
	match anim_state:
		ANIMATION_STATES.IDLE:
			if Input.is_action_just_pressed("dodge"):
				anim_state = ANIMATION_STATES.DODGE
			elif velocity.y != 0:
				anim_state = ANIMATION_STATES.JUMP
			elif Input.is_action_pressed("crouch"):
				anim_state = ANIMATION_STATES.CROUCH
			elif velocity.length() > 0:
				anim_state = ANIMATION_STATES.WALK

		ANIMATION_STATES.WALK:
			if Input.is_action_just_pressed("dodge"):
				anim_state = ANIMATION_STATES.DODGE
			elif velocity.y != 0:
				anim_state = ANIMATION_STATES.JUMP
			elif Input.is_action_pressed("crouch"):
				anim_state = ANIMATION_STATES.CROUCH
			elif velocity.length() == 0:
				anim_state = ANIMATION_STATES.IDLE

		ANIMATION_STATES.JUMP:
			if Input.is_action_just_pressed("dodge"):
				anim_state = ANIMATION_STATES.DODGE
			elif is_on_floor():
				if velocity.length() > 0:
					anim_state = ANIMATION_STATES.WALK
				elif velocity.length() == 0:
					anim_state = ANIMATION_STATES.IDLE

		ANIMATION_STATES.CROUCH:
			if Input.is_action_just_pressed("dodge"):
				anim_state = ANIMATION_STATES.DODGE
			elif velocity.y != 0:
				anim_state = ANIMATION_STATES.JUMP
			elif !Input.is_action_pressed("crouch"):
				if velocity.length() == 0:
					anim_state = ANIMATION_STATES.IDLE
				elif velocity.length() > 0:
					anim_state = ANIMATION_STATES.WALK

	var current_anim: StringName = $sprite.animation
	# logica_salida:
	match anim_state:
		ANIMATION_STATES.IDLE:
			if current_anim != "idle":
				$sprite.play("idle")
		ANIMATION_STATES.WALK:
			if current_anim != "walk":
				$sprite.play("walk")
		ANIMATION_STATES.JUMP:
			if current_anim != "jump":
				$sprite.play("jump")
			if Input.is_action_just_pressed("jump")&& jumps_left != 0:
				$sprite.play("idle") # Pequeño hack para que la animación de jump vuelva a empezar
				$sprite.play("jump")
		ANIMATION_STATES.CROUCH:
			if current_anim != "crouch":
				$sprite.play("crouch")
		ANIMATION_STATES.DIE:
			if current_anim != "die":
				$sprite.play("die")
		ANIMATION_STATES.DODGE:
			if current_anim != "dodge":
				$sprite.play("dodge")

# Actualizar máquina de estados de dirección
func player_update_facing() -> void:
	$sprite.set_flip_h(facing==FACING.RIGHT)

# Actualiza la máquina de estados del cambio de anillo
func player_change_ring_state() -> void:
	if changing_ring:
		return

	changing_ring = true
	if curr_ring == RING.EXTERIOR:
		curr_ring = RING.INTERIOR
	else:
		curr_ring = RING.EXTERIOR

func player_change_level_state() -> void:
	match curr_level:
		LEVEL.LOWER:
			target_level = LEVEL.MIDDLE
		LEVEL.MIDDLE:
			target_level = LEVEL.UPPER


# ======== Consultoras ========
func player_is_crouching() -> bool:
	return Input.is_action_pressed("crouch") && is_on_floor()

func player_should_die() -> bool:
	return Input.is_action_pressed("dbg_die")

func player_is_dead() -> bool:
	return anim_state == ANIMATION_STATES.DIE

func player_is_dodging() -> bool:
	return anim_state == ANIMATION_STATES.DODGE

func player_handle_input() -> void:
	if Input.is_action_just_pressed("dbg_switch_ring"):
		player_change_ring_state()
	if Input.is_action_just_pressed("dbg_next_level"):
		if resetting_alpha:
			return

		resetting_alpha = true
	if Input.is_action_just_pressed("dbg_reset_position"):
		player_reset_position()

# ======== Actuadoras ========
func player_die() -> void:
	anim_state = ANIMATION_STATES.DIE

func player_switch_ring() -> void:
	if curr_ring == RING.EXTERIOR:

		# Target ring is exterior, if were in the first frame of the change, jump.
		# This way its only done once. Otherwise, itll keep adding y velocity eternally.
		if player_radius == radius_interior && is_on_floor():
			velocity.y = RING_SWITCH_JUMP_VELOCITY
		elif player_radius == radius_exterior:
			# If the target ring is the exterior one and were already there, stop the thing.
			changing_ring = false

		if changing_ring:
			# Gradually change the player_radius to get a smooth transition.
			# As seen above, we stop adding as soon as we reach the target radius.
			player_radius += RING_SWITCH_SPEED
	else:
		if player_radius == radius_exterior and is_on_floor():
			velocity.y = RING_SWITCH_JUMP_VELOCITY
		elif player_radius == radius_interior:
			changing_ring = false

		if changing_ring:
			player_radius -= RING_SWITCH_SPEED

func player_switch_level() -> void:
	if curr_level == target_level:
		return

	$collision.disabled = true

	if get_position().y < LEVEL_HEIGHTS[target_level]:
		if is_on_floor():
			velocity.y = RING_SWITCH_JUMP_VELOCITY*2.3
	else:
		$collision.disabled = false
		curr_level = target_level

func player_reset_position() -> void:
	if player_is_dead():
		return
	if changing_ring:
		return
	if curr_level != target_level:
		return

	entity_alpha = 0
	curr_ring = RING.EXTERIOR
	curr_level = LEVEL.LOWER
	target_level = curr_level
	velocity = Vector3(0, 0, 0)
	transform.origin.y = 5
	player_radius = radius_exterior
	$collision.disabled = false

# ======== Callbacks ========
func player_on_animation_finished() -> void:
	match anim_state:
		ANIMATION_STATES.DODGE:
			anim_state = ANIMATION_STATES.IDLE
			$sprite.play("idle")


func player_jump():
	jumps_left -= 1
	velocity.y = JUMP_VELOCITY


# ======== Reimplementaciones de entity ========
func entity_should_jump() -> bool:
	return Input.is_action_just_pressed("jump") and jumps_left > 0
