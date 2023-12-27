class_name Player extends GenericEntity

# scenes
var bullet_pistol = preload("res://bullet_pistol.tscn")
var bullet_rifle = preload("res://bullet_rifle.tscn")

# Enumerations
enum ANIMATION_STATES { IDLE, WALK, JUMP, CROUCH, DIE, DODGE }
enum FACING { LEFT=-1, RIGHT=1 }
enum WEAPON { PISTOL, RIFLE }
enum RING { EXTERIOR, INTERIOR }
enum LEVEL { LOWER, MIDDLE, UPPER }
var LEVEL_HEIGHTS = [ 0, 20, 30 ]

# Máquinas de estados
var anim_state = ANIMATION_STATES.IDLE
var curr_ring = RING.EXTERIOR
var changing_ring : bool = false
var curr_level : LEVEL = LEVEL.LOWER
var target_level : LEVEL = curr_level
var resetting_alpha : bool = false
var active_weapon : WEAPON = WEAPON.PISTOL
var facing: FACING = FACING.LEFT

# Velocidades angulares del jugador
@export var SPEED = PI/8 # 1 lap = 16 secs.
@export var DODGE_SPEED = PI/2

# Radios de anillos y velocidad de cambio
@export var RING_SWITCH_SPEED = 0.125
const radius_exterior = 20
const radius_interior = 15

# Jump parameters
const JUMP_VELOCITY = 7
const RING_SWITCH_JUMP_VELOCITY = JUMP_VELOCITY*1.2
const INIT_JUMPS_LEFT = 2
var jumps_left: int = INIT_JUMPS_LEFT # Cuenta el numero de saltos que puede dar el jugador

# Salud
const INIT_HEALTH = 100
var health = INIT_HEALTH
var god_mode = false

# ======== Reimplementaciones de funciones de CharacterBody3D ========

func _ready() -> void:
	player_init_sprites()
	player_reset_position()

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

	# Changing ring
	if changing_ring:
		player_switch_ring()

	# Changing level
	if curr_level != target_level:
		player_switch_level()

	# Handle jump logic.
	if is_on_floor():
		jumps_left = INIT_JUMPS_LEFT

	if !is_on_floor() && jumps_left == INIT_JUMPS_LEFT:
		jumps_left = 1

	super(delta)



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

	var current_anim: StringName = player_get_current_active_sprite().animation

	# logica_salida:
	match anim_state:
		ANIMATION_STATES.IDLE:
			if current_anim != "idle":
				player_play_animation("idle")
		ANIMATION_STATES.WALK:
			if current_anim != "walk":
				player_play_animation("walk")
		ANIMATION_STATES.JUMP:
			if current_anim != "jump":
				player_play_animation("jump")
			if Input.is_action_just_pressed("jump")&& jumps_left != 0:
				player_play_animation("idle") # Pequeño hack para que la animación de jump vuelva a empezar
				player_play_animation("jump")
		ANIMATION_STATES.CROUCH:
			if current_anim != "crouch":
				player_play_animation("crouch")
		ANIMATION_STATES.DIE:
			if current_anim != "die":
				player_play_animation("die")
		ANIMATION_STATES.DODGE:
			if current_anim != "dodge":
				player_play_animation("dodge")
				if !is_on_floor():
					velocity.y -= JUMP_VELOCITY*2

# Actualizar máquina de estados de dirección
func player_update_facing() -> void:
	$sprite_pistol.set_flip_h(facing==FACING.RIGHT)
	$sprite_rifle.set_flip_h(facing==FACING.RIGHT)

# Actualiza la máquina de estados del cambio de anillo
func player_change_ring_state() -> void:
	if changing_ring:
		return

	changing_ring = true
	if curr_ring == RING.EXTERIOR:
		curr_ring = RING.INTERIOR
	else:
		curr_ring = RING.EXTERIOR

# Actualiza el estado del anillo en el que debemos llegar
func player_change_level_state() -> void:
	match curr_level:
		LEVEL.LOWER:
			target_level = LEVEL.MIDDLE
		LEVEL.MIDDLE:
			target_level = LEVEL.UPPER


# ======== Consultoras ========
# Dice si el jugador está crouching
func player_is_crouching() -> bool:
	return Input.is_action_pressed("crouch") && is_on_floor()

# Determina si el jugador debe morir
func player_should_die() -> bool:
	return health <= 0

# Determina si el jugador ha muerto
func player_is_dead() -> bool:
	return anim_state == ANIMATION_STATES.DIE

# Dice si el jugador está en animación "DODGE"
func player_is_dodging() -> bool:
	return anim_state == ANIMATION_STATES.DODGE

# Devuelve si el jugador se debería mover
func player_should_move() -> bool:
	var move_right = Input.is_action_pressed("move_right")
	var move_left  = Input.is_action_pressed("move_left" )
	return (move_right != move_left) || player_is_dodging()

# Devuelve la dirección de movimiento segun input
func player_move_dir() -> EntityDirection:
	if Input.is_action_just_pressed("move_right"):
		return EntityDirection.RIGHT
	elif Input.is_action_pressed("move_left"):
		return EntityDirection.LEFT
	return EntityDirection.NONE

# Ahora por ahora, solamente funciona para input de debug
func player_handle_input() -> void:
	if Input.is_action_just_pressed("god_mode"):
		god_mode = !god_mode
	if Input.is_action_just_pressed("dbg_die"):
		health = 0
	if Input.is_action_just_pressed("dbg_take_damage"):
		player_take_damage(20)
	if Input.is_action_just_pressed("dbg_switch_ring"):
		player_change_ring_state()
	if Input.is_action_just_pressed("dbg_next_level"):
		if resetting_alpha:
			return
		resetting_alpha = true
	if Input.is_action_just_pressed("dbg_reset_position"):
		player_reset_position()
	if Input.is_action_just_pressed("dbg_switch_weapon"):
		if active_weapon == WEAPON.PISTOL:
			$sprite_pistol.hide()
			$sprite_rifle.show()
			active_weapon = WEAPON.RIFLE
		else:
			$sprite_pistol.show()
			$sprite_rifle.hide()
			active_weapon = WEAPON.PISTOL

	if Input.is_action_just_pressed("shoot"):
		player_shoot()

# ======== Actuadoras ========
func player_die() -> void:
	anim_state = ANIMATION_STATES.DIE

func player_switch_ring() -> void:
	if curr_ring == RING.EXTERIOR:

		# Target ring is exterior, if were in the first frame of the change, jump.
		# This way its only done once. Otherwise, itll keep adding y velocity eternally.
		if entity_radius == radius_interior && is_on_floor():
			velocity.y = RING_SWITCH_JUMP_VELOCITY
		elif entity_radius == radius_exterior:
			# If the target ring is the exterior one and were already there, stop the thing.
			changing_ring = false

		if changing_ring:
			# Gradually change the player_radius to get a smooth transition.
			# As seen above, we stop adding as soon as we reach the target radius.
			entity_radius += RING_SWITCH_SPEED
	else:
		if entity_radius == radius_exterior and is_on_floor():
			velocity.y = RING_SWITCH_JUMP_VELOCITY
		elif entity_radius == radius_interior:
			changing_ring = false

		if changing_ring:
			entity_radius -= RING_SWITCH_SPEED

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
	entity_radius = radius_exterior
	curr_ring = RING.EXTERIOR
	curr_level = LEVEL.LOWER
	target_level = curr_level
	velocity = Vector3(0, 0, 0)
	transform.origin.y = 5
	health = INIT_HEALTH
	$collision.disabled = false

func player_play_animation(_anim: StringName) -> void:
	$sprite_pistol.play(_anim)
	$sprite_rifle.play(_anim)

func player_shoot() -> void:
	if player_is_dead():
		return

	if player_is_dodging():
		return

	if changing_ring:
		return

	velocity.y = 0

	var bullet = null

	match active_weapon:
		WEAPON.PISTOL:
			bullet = bullet_pistol
		WEAPON.RIFLE:
			bullet = bullet_rifle

	var b = bullet.instantiate()
	var pos = get_position()

	b.init(pos, entity_alpha, facing, entity_radius, player_is_crouching())
	owner.add_child(b)


func player_take_damage(damage: int) -> void:
	if god_mode:
		return
	health -= damage

# ======== Callbacks ========
func player_on_animation_finished() -> void:
	match anim_state:
		ANIMATION_STATES.DODGE:
			anim_state = ANIMATION_STATES.IDLE
			$sprite_pistol.play("idle")
			$sprite_rifle.play("idle")

# ======== Utils ========
func player_get_current_active_sprite() -> AnimatedSprite3D:
	if active_weapon == WEAPON.PISTOL:
		return $sprite_pistol
	else:
		return $sprite_rifle

# ======== Initializations ========
func player_init_sprites() -> void:
	$sprite_pistol.set_scale($sprite_pistol.scale)
	$sprite_rifle.set_scale($sprite_rifle.scale)
	$sprite_pistol.connect("animation_finished", player_on_animation_finished)
	$sprite_rifle.connect("animation_finished", player_on_animation_finished)
	$sprite_rifle.hide()

	$sprite_pistol.play("idle")


# ======== Reimplementaciones de entity ========
# VIRTUAL. TO BE OVERRIDEN
# Retorna si la entidad debe saltar
func entity_should_jump() -> bool:
	return Input.is_action_just_pressed("jump") and jumps_left > 0

# VIRTUAL. TO BE OVERRIDEN
# Retorna la velocidad de salto de la entidad
# SIDE EFFECT: toca las variables que sean necesarias de lógica en la implementación
func entity_jump() -> float:
	jumps_left -= 1
	return JUMP_VELOCITY

# VIRTUAL. TO BE OVERRIDEN
# Retorna el siguiente alpha, en base al alpha
# actual, la dirección, y el delta
func entity_get_new_alpha(current_alpha: float, direction: EntityDirection, delta: float) -> float:
	# Update entity_alpha
	var next_alpha = current_alpha
	if resetting_alpha:
		# Autopilot. Vamos a lo que vamos
		var speed: float = SPEED*15
		next_alpha += (direction * speed * delta)
		if (current_alpha >= -0.05 && current_alpha <= 0.05 ) || (next_alpha > 0 && current_alpha < 0) or (next_alpha < 0 and current_alpha > 0):
			next_alpha = 0
			resetting_alpha = false
			player_change_level_state()
		# else:
		# 	next_alpha = next_alpha
	else:
		if !player_is_crouching():
			var speed: float = DODGE_SPEED if player_is_dodging() else SPEED
			var real_dir: int = facing if player_is_dodging() else direction
			# only move when keys are pressed
			if player_should_move():
				next_alpha += real_dir * speed * delta
			if next_alpha > 2*PI:
				next_alpha -= 2*PI
			elif next_alpha < -2*PI:
				next_alpha += 2*PI

	return next_alpha

# VIRTUAL. TO BE OVERRIDEN
# Retorna la siguiente dirección
func entity_get_new_direction(current_direction: EntityDirection) -> EntityDirection:
	var right_pressed = Input.is_action_pressed("move_right")
	var left_pressed = Input.is_action_pressed("move_left")
	# Get direction of movement
	if resetting_alpha:
		# No hacemos update de facing.
		# Se queda igual si estamos en autopilot
		if entity_alpha > 0:
			return EntityDirection.LEFT
		else:
			return EntityDirection.RIGHT
	elif right_pressed && !left_pressed:
		facing = FACING.RIGHT
		return EntityDirection.RIGHT
	elif left_pressed && !right_pressed:
		facing = FACING.LEFT
		return EntityDirection.LEFT
	else:
		return current_direction
