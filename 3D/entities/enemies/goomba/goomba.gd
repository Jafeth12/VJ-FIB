class_name Goomba extends GenericEnemy

var attack_ended: bool = false
var player_in_area: bool = false

const SPEED: float = PI/10
const ATTACK_SPEED: float = PI/4
@export var ATTACK_DAMAGE: int = 10

var player_node = null

func _ready():
	ENEMY_INIT_SHIELD = 200
	ENEMY_INIT_HEALTH = 130
	enemy_init_bars($SubViewport/ShieldBar3D, $SubViewport/HealthBar3D)
	$damage_area.connect("body_entered", goomba_damage_area_entered)
	player_node = get_node("/root/main/Player")
	super()

# OVERRIDE de ENTITY
func entity_get_new_direction(current_direction: EntityDirection) -> EntityDirection:
	var prx_dir: EntityDirection = current_direction
	match enemy_state:
		EnemyState.WANDER:
			if is_on_wall():
				prx_dir = -current_direction
	$sprite.set_flip_h(current_direction==EntityDirection.RIGHT)
	return prx_dir

# OVERRIDE de ENTITY
func entity_get_new_alpha(current_alpha: float, direction: EntityDirection, delta: float) -> float:
	match enemy_state:
		EnemyState.WANDER:
			return current_alpha + direction * SPEED * delta
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
			# Aquí no llega NUNCA
			if $sprite.animation != "die":
				$sprite.play("die")
		EnemyState.DEAD:
			if $sprite.animation != "die":
				$sprite.play("die")

# OVERRIDE de ENEMY
func enemy_should_attack() -> bool:
	return false # NUNCA ataca, pq siempre hace daño

# OVERRIDE de ENEMY
func enemy_is_attack_finished() -> bool:
	return true # Preventivo. Por si se pone en modo ataque si querer

func goomba_damage_area_entered(_body: Node3D):
	if enemy_state != EnemyState.DEAD:
		player_node.player_take_damage(ATTACK_DAMAGE)
