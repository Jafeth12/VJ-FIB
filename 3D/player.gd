extends CharacterBody3D

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
const JUMP_VELOCITY = 7
const RING_SWITCH_JUMP_VELOCITY = JUMP_VELOCITY*1.5

# Físicas comúnes
# var gravity = ProjectSettings.get_setting("physics/3d/default_gravity")
const gravity = 15 # TODO: abstraer esto

# Movimiento circular y alpha
@export var alpha = 0
var old_alpha = 0

# Reimplementaciones de funciones de CharacterBody3D
func _ready() -> void:
	$sprite.set_scale($sprite.scale)
	$sprite.play("idle")
	$sprite.connect("animation_finished", on_animation_finished)

# Gestionar la lógica que no tiene que ver con la física del jugador
# p.e.: Vida y muerte, cambio de anillo, animaciones, etc.
func _process(delta: float) -> void:
	if should_die():
		die()
	if !is_dead():
		update_facing()
	handle_input()
	look_at(Vector3(0, get_position().y, 0))
	update_anim_state()

# Gestionar todo aquello relacionado con el movimiento del jugador.
# p.e.: Movimiento angular, saltos, colisiones, etc.
func _physics_process(delta: float) -> void:
	if is_dead():
		return
	# Add the gravity.
	if not is_on_floor():
		velocity.y -= gravity * delta

	if changing_ring:
		switch_ring()

	if curr_level != target_level:
		switch_level()

	# Handle jump.
	if Input.is_action_just_pressed("jump") and is_on_floor():
		velocity.y = JUMP_VELOCITY

	# Get the input direction and handle the movement/deceleration.
	# As good practice, you should replace UI actions with custom gameplay actions.
	var right_pressed: bool = Input.is_action_pressed("move_right")
	var left_pressed: bool = Input.is_action_pressed("move_left")

	# Get direction of movement
	var circular_dir: int = 0;
	if resetting_alpha:
		if alpha > 0:
			circular_dir = -1
		else:
			circular_dir = 1
	elif (right_pressed && !left_pressed):
		circular_dir = 1
		facing = FACING.RIGHT
	elif left_pressed && !right_pressed:
		circular_dir = -1
		facing = FACING.LEFT

	# Update alpha
	if resetting_alpha:
		var speed: float = SPEED*15
		var real_dir: int = circular_dir
		var next_alpha: float = alpha + (real_dir * speed * delta)
		if (alpha >= -0.05 && alpha <= 0.05 ) || (next_alpha > 0 && alpha < 0) or (next_alpha < 0 and alpha > 0):
			alpha = 0
			resetting_alpha = false
			change_level_state()
		else:
			alpha = next_alpha
	else:
		if !is_crouching():
			var speed: float = DODGE_SPEED if is_dodging() else SPEED
			var real_dir: int = circular_dir if !is_dodging() else int(facing)
			alpha += real_dir * speed * delta
			if alpha > 2*PI:
				alpha -= 2*PI
			elif alpha < -2*PI:
				alpha += 2*PI

	var next_xz = get_next_xz()
	velocity.x = (next_xz.x - get_position().x)/delta
	velocity.z = (next_xz.y - get_position().z)/delta

	var length: float = velocity.length()
	if length < 0.005:
		velocity = Vector3(0, 0, 0)

	move_and_slide()
	if is_on_wall():
		alpha = old_alpha
		alpha = get_position_alpha(get_position())
	old_alpha = alpha

# Actualizar la máquina de estados de las animacoines.
# Se activa la animación si hace falta (lógica de salida)
func update_anim_state():
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
func update_facing() -> void:
	$sprite.set_flip_h(facing==FACING.RIGHT)

# Actualiza la máquina de estados del cambio de anillo
func change_ring_state() -> void:
	if changing_ring:
		return

	changing_ring = true
	if curr_ring == RING.EXTERIOR:
		curr_ring = RING.INTERIOR
	else:
		curr_ring = RING.EXTERIOR

func change_level_state() -> void:
	match curr_level:
		LEVEL.LOWER:
			target_level = LEVEL.MIDDLE
		LEVEL.MIDDLE:
			target_level = LEVEL.UPPER

# Devuelve las coordenadas xz en base al ángulo alpha actual del jugador
func get_next_xz() -> Vector2:
	return Vector2(player_radius*sin(alpha), player_radius*cos(alpha))

# Devuelve el ángulo alpha en base a la posición del jugador
func get_position_alpha(_pos: Vector3) -> float:
	return old_alpha
	# alpha = asin(pos.x/player_radius)
	# alpha = acos(pos.z/player_radius)

# ======== Consultoras ========
func is_crouching() -> bool:
	return Input.is_action_pressed("crouch") && is_on_floor()
func should_die() -> bool:
	return Input.is_action_pressed("dbg_die")
func is_dead() -> bool:
	return anim_state == ANIMATION_STATES.DIE
func is_dodging() -> bool:
	return anim_state == ANIMATION_STATES.DODGE

func handle_input() -> void:
	if Input.is_action_just_pressed("dbg_switch_ring"):
		change_ring_state()
	if Input.is_action_just_pressed("dbg_next_level"):
		if resetting_alpha:
			return

		resetting_alpha = true
	if Input.is_action_just_pressed("dbg_reset_position"):
		reset_position()

# ======== Actuadoras ========
func die() -> void:
	anim_state = ANIMATION_STATES.DIE

func switch_ring() -> void:
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

func switch_level() -> void:
	if curr_level == target_level:
		return

	$collision.disabled = true

	if get_position().y < LEVEL_HEIGHTS[target_level]:
		if is_on_floor():
			velocity.y = RING_SWITCH_JUMP_VELOCITY*2.3
	else:
		$collision.disabled = false
		curr_level = target_level

func reset_position() -> void:
	if is_dead():
		return
	if changing_ring:
		return
	if curr_level != target_level:
		return

	alpha = 0
	curr_ring = RING.EXTERIOR
	curr_level = LEVEL.LOWER
	target_level = curr_level
	velocity = Vector3(0, 0, 0)
	transform.origin.y = 5
	player_radius = radius_exterior
	$collision.disabled = false

# ======== Callbacks ========
func on_animation_finished() -> void:
	match anim_state:
		ANIMATION_STATES.DODGE:
			anim_state = ANIMATION_STATES.IDLE
			$sprite.play("idle")
