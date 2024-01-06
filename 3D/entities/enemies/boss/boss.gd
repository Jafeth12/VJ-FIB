class_name Boss extends GenericEnemy

var attack_ended: bool = false
var player_in_area: bool = false

const SPEED: float = PI/10
const ATTACK_SPEED: float = PI/4
@export var ATTACK_DAMAGE: int = 20
@export var LEVEL: MainLogic.LEVEL = MainLogic.LEVEL.LEVEL1

var player_node = null

func _ready():
	match LEVEL:
		MainLogic.LEVEL.LEVEL1:
			enemy_drop_chance = 1
		MainLogic.LEVEL.LEVEL2 || MainLogic.LEVEL.END:
			enemy_drop_chance = 0
	ENEMY_INIT_SHIELD = 1000
	ENEMY_INIT_HEALTH = 500
	enemy_init_bars($SubViewport/ShieldBar3D, $SubViewport/HealthBar3D)
	$sprite.connect("animation_finished", boss_on_animation_finished)
	$activation_area.connect("body_entered", boss_area_entered)
	$activation_area.connect("body_exited", boss_area_exited)
	$damage_area.connect("body_entered", boss_damage_area_entered)
	player_node = get_node("/root/main/Player")
	super()

# OVERRIDE de ENTITY
func entity_get_new_direction(current_direction: EntityDirection) -> EntityDirection:
	var prx_dir: EntityDirection = current_direction
	match enemy_state:
		EnemyState.WANDER:
			if player_in_area:
				prx_dir = enemy_get_direction_to_player(player_node)
			if is_on_wall():
				prx_dir = -current_direction
		EnemyState.ATTACK:
			if $sprite.frame < 3:
				prx_dir = enemy_get_direction_to_player(player_node)
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
func enemy_is_attack_finished() -> bool:
	var ae_copy: bool = attack_ended
	attack_ended = false
	return ae_copy

func boss_is_damaging():
	return enemy_state == EnemyState.ATTACK && $sprite.frame >= 3

func boss_on_animation_finished() -> void:
	match $sprite.animation:
		"attack":
			attack_ended = true

func boss_area_entered(body: Node3D):
	if body is StaticBody3D:
		return
	player_in_area = true

func boss_area_exited(_body: Node3D):
	player_in_area = false

func boss_damage_area_entered(_body: Node3D):
	if boss_is_damaging():
		player_node.player_take_damage(ATTACK_DAMAGE)
