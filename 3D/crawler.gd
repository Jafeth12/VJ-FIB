class_name Crawler extends GenericEnemy

var attack_ended: bool = false
var player_in_area: bool = false

@export var INIT_ALPHA = 0

const SPEED: float = PI/10
const ATTACK_SPEED: float = PI/4

var player_node = null

func _ready():
	entity_alpha = INIT_ALPHA
	$sprite.connect("animation_finished", crawler_on_animation_finished)
	$activation_area.connect("area_entered", crawler_area_entered)
	$activation_area.connect("area_exited", crawler_area_exited)
	player_node = get_node("/root/main/level1/Player")

func modf(a: float, d: float) -> float:
	var sign: int = 1 if a > 0 else -1
	a = abs(a)
	while a >= d:
		a -= d
	return sign*a

# OVERRIDE de ENTITY
func entity_get_new_direction(current_direction: EntityDirection) -> EntityDirection:
	var prx_dir: EntityDirection = current_direction
	match enemy_state:
		EnemyState.WANDER:
			if player_in_area:
				var angular_separation: float = modf(abs(player_node.alpha - entity_alpha), 2*PI)
				print(angular_separation)
				if angular_separation < PI:
					# angle1 is to the left of angle2
					print("ME TIRO A LA IZQUIERDA")
					prx_dir = EntityDirection.LEFT
				else:
					#angle1 is to the right of angle2
					print("ME TIRO A LA DERECHA")
					prx_dir = EntityDirection.RIGHT
			if is_on_wall():
				prx_dir = -current_direction

	$sprite.set_flip_h(current_direction==EntityDirection.RIGHT)
	return prx_dir

# OVERRIDE de ENTITY
func entity_get_new_alpha(current_alpha: float, direction: EntityDirection, delta: float) -> float:
	match enemy_state:
		EnemyState.WANDER:
			return current_alpha + direction * SPEED * delta
		EnemyState.ATTACK:
			if $sprite.frame > 3:
				return current_alpha + direction * ATTACK_SPEED * delta
	return current_alpha

# OVERRIDE de ENEMY
func enemy_update_animation() -> void:
	match enemy_state:
		EnemyState.WAIT:
			if $sprite.animation != "idle":
				$sprite.play("idle")
		EnemyState.WANDER:
			if $sprite.animation != "walk":
				$sprite.play("walk")
		EnemyState.ATTACK:
			if $sprite.animation != "attack":
				$sprite.play("attack")
		EnemyState.DEAD:
			if $sprite.animation != "die":
				$sprite.play("die")

# OVERRIDE de ENEMY
func enemy_should_attack() -> bool:
	return player_in_area

# OVERRIDE de ENEMY
func enemy_should_die() -> bool:
	return Input.is_action_just_pressed("dbg_enemies_die")

# OVERRIDE de ENEMY
func enemy_is_attack_finished() -> bool:
	var ae_copy: bool = attack_ended
	attack_ended = false
	return ae_copy

func crawler_on_animation_finished() -> void:
	match $sprite.animation:
		"attack":
			attack_ended = true

func crawler_area_entered(area: Area3D):
	print("entró")
	player_in_area = true

func crawler_area_exited(area: Area3D):
	print("salió")
	player_in_area = false
