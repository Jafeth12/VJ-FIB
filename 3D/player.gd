extends CharacterBody3D

enum ANIMATION_STATES { IDLE, WALK, JUMP, CROUCH, DIE, DODGE }
enum FACING { LEFT=-1, RIGHT=1 }

@export var SPEED = PI/8 # 1 lap = 16 secs.
@export var DODGE_SPEED = PI/2
const JUMP_VELOCITY = 7

# Get the gravity from the project settings to be synced with RigidBody nodes.
# var gravity = ProjectSettings.get_setting("physics/3d/default_gravity")
const gravity = 15 # TODO: abstraer esto
const player_radius = 18 # TODO: abtraer esto
@export var alpha = 0
var old_alpha = 0

@export var our_scale = 3

var anim_state = ANIMATION_STATES.IDLE
var facing = FACING.RIGHT

func _ready() -> void:
	$sprite.set_scale(Vector3(our_scale, our_scale, our_scale))
	$sprite.play("idle")
	$sprite.connect("animation_finished", on_animation_finished)

func _process(delta: float) -> void:
	if should_die():
		die()
	if !is_dead():
		update_facing()
	look_at(Vector3(0, get_position().y, 0))
	update_anim_state()

func _physics_process(delta: float) -> void:
	if is_dead():
		return
	# Add the gravity.
	if not is_on_floor():
		velocity.y -= gravity * delta

	# Handle jump.
	if Input.is_action_just_pressed("jump") and is_on_floor():
		velocity.y = JUMP_VELOCITY

	# Get the input direction and handle the movement/deceleration.
	# As good practice, you should replace UI actions with custom gameplay actions.
	var right_pressed: bool = Input.is_action_pressed("move_right")
	var left_pressed: bool = Input.is_action_pressed("move_left")

	# Get direction of movement
	var circular_dir: int = 0;
	if right_pressed && !left_pressed:
		circular_dir = 1
		facing = FACING.RIGHT
	elif left_pressed && !right_pressed:
		circular_dir = -1
		facing = FACING.LEFT

	# Update alpha
	if !is_crouching():
		var speed: float = DODGE_SPEED if is_dodging() else SPEED
		var real_dir: int = circular_dir if !is_dodging() else int(facing)
		alpha += real_dir * speed * delta
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

func update_anim_state():
	var current_anim: StringName = $sprite.animation
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

func update_facing() -> void:
	if facing == FACING.RIGHT:
		$sprite.set_scale(Vector3(-our_scale, our_scale, our_scale))
	elif facing == FACING.LEFT:
		$sprite.set_scale(Vector3(our_scale, our_scale, our_scale))

func get_next_xz() -> Vector2:
	return Vector2(player_radius*sin(alpha), player_radius*cos(alpha))

func get_position_alpha(_pos: Vector3) -> float:
	return old_alpha
	# alpha = asin(pos.x/player_radius)
	# alpha = acos(pos.z/player_radius)

func is_crouching() -> bool:
	return Input.is_action_pressed("crouch") && is_on_floor()

func should_die() -> bool:
	return Input.is_action_pressed("dbg_die")

func die() -> void:
	anim_state = ANIMATION_STATES.DIE

func is_dead() -> bool:
	return anim_state == ANIMATION_STATES.DIE

func is_dodging() -> bool:
	return anim_state == ANIMATION_STATES.DODGE

func on_animation_finished() -> void:
	match anim_state:
		ANIMATION_STATES.DODGE:
			anim_state = ANIMATION_STATES.IDLE
