class_name GenericEnemy extends GenericEntity

# Estado del enemigo
enum EnemyState { WAIT, WANDER, ATTACK, DEAD }
var enemy_state: EnemyState = EnemyState.WAIT

var ENEMY_INIT_SHIELD = 100
var ENEMY_INIT_HEALTH = 100
var enemy_shield = ENEMY_INIT_SHIELD
var enemy_health = ENEMY_INIT_HEALTH

var enemy_shield_bar: ProgressBar = null
var enemy_health_bar: ProgressBar = null

var drop_item = preload("res://entities/other/drop.tscn")

@export var enemy_drop_type: Enums.DROP_TYPE = Enums.DROP_TYPE.HEALTH
@export var enemy_drop_amount: int = 10
@export var enemy_drop_chance: float = 0.5
@export var enemy_damage_sound: AudioStreamPlayer3D
@export var enemy_attack_sound: AudioStreamPlayer3D
@export var enemy_death_sound: AudioStreamPlayer3D

signal enemy_died(enemy)

func _ready():
	super()
	enemy_shield = ENEMY_INIT_SHIELD
	enemy_health = ENEMY_INIT_HEALTH
	enemy_damage_sound.bus = "enemies"
	enemy_attack_sound.bus = "enemies"
	enemy_death_sound.bus = "enemies"

func _process(_delta):
	look_at(Vector3(0, get_position().y, 0))
	enemy_update_state()
	enemy_update_animation()

func enemy_init_bars(shield_bar_id, health_bar_id) -> void:
	enemy_shield_bar = shield_bar_id
	enemy_shield_bar.max_value = ENEMY_INIT_SHIELD
	enemy_shield_bar.value = ENEMY_INIT_SHIELD
	enemy_health_bar = health_bar_id
	enemy_health_bar.max_value = ENEMY_INIT_HEALTH
	enemy_health_bar.value = ENEMY_INIT_HEALTH
	enemy_shield_bar.show()
	enemy_health_bar.hide()

# Updates enemy state
# This is a generic state machine and could be reimplemented
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
				enemy_attack_sound.play()
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

# Enemy takes damage
func enemy_take_damage(damage: int) -> void:
	if enemy_shield > 0:
		if damage > enemy_shield:
			# El daño es mayor que el escudo restante.
			# Restamos al daño inflingido el escudo restante,
			# y lo ponemos a 0. Después, restamos el resto de
			# daño a la salud.
			damage -= enemy_shield
			enemy_shield = 0
			enemy_health -= damage
		else:
			enemy_shield -= damage
	else:
		enemy_health -= damage
	
	enemy_update_health_bar()

	if enemy_is_dead():
		emit_signal("enemy_died", self)

		var chance = randf_range(0, 1)
		var drops = chance <= enemy_drop_chance

		if !drops:
			return

		var drop = drop_item.instantiate()
		var new_pos = get_position()
		new_pos.z += 0.4
		drop.set_position(new_pos)
		drop.set_drop_type(enemy_drop_type)
		drop.set_drop_amount(enemy_drop_amount)
		drop.apply_central_impulse(Vector3(0, 6, 0))
		get_parent().add_child(drop)
		
		enemy_attack_sound.stop()
		enemy_damage_sound.stop()
		enemy_death_sound.play()
	else:
		enemy_damage_sound.play()

func enemy_update_health_bar() -> void:
	if enemy_is_dead():
		enemy_shield_bar.hide()
		enemy_health_bar.hide()
		return
	
	if enemy_shield > 0:
		enemy_shield_bar.show()
		enemy_health_bar.hide()
	else:
		enemy_shield_bar.hide()
		enemy_health_bar.show()
	
	enemy_shield_bar.value = enemy_shield
	enemy_health_bar.value = enemy_health


# Use this function to see if the enemy should die
# p.e.: if it's life is empty
func enemy_should_die() -> bool:
	return enemy_shield <= 0 && enemy_health <= 0

# VIRTUAL. TO BE OVERRIDEN
func enemy_update_animation() -> void:
	pass

# VIRTUAL. TO BE OVERRIDEN
# Return if the player should activate when it is in WAIT state
# p.e.: When the player is on the same platform as you
func enemy_should_activate() -> bool:
	return true

# VIRTUAL. TO BE OVERRIDEN
# Use this function to see if the enemy should attack the player
# p.e.: if the player is close enough
func enemy_should_attack() -> bool:
	return false

# VIRTUAL. TO BE OVERRIDEN
# Use this function to indicate that the enemy has finished
# its attack
# p.e.: when the attack animation is finished
func enemy_is_attack_finished() -> bool:
	return false

func enemy_is_dead() -> bool:
	return enemy_shield <= 0 && enemy_health <= 0

func is_same_sign(a: float, b: float) -> bool:
	return (a < 0 && b < 0) || (a >= 0 && b >= 0)

func enemy_get_direction_to_player(player_node):
	# Normalize angles to the range (-180, 180]
	var a = fposmod(entity_alpha+PI, 2.0*PI) - PI
	var b = fposmod(player_node.entity_alpha+PI, 2.0*PI) - PI
	# Calculate the shortest angular distance
	var distance = fposmod((b - a + PI), 2.0*PI) - PI
	# Deduce direction, based on distance sign
	return EntityDirection.LEFT if distance < 0 else EntityDirection.RIGHT
