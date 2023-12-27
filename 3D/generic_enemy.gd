class_name GenericEnemy extends GenericEntity

# Estado del enemigo
enum EnemyState { WAIT, WANDER, ATTACK, DEAD }
var enemy_state: EnemyState = EnemyState.WAIT

const ENEMY_INIT_SHIELD = 100
const ENEMY_INIT_HEALTH = 100
var enemy_shield = ENEMY_INIT_SHIELD
var enemy_health = ENEMY_INIT_HEALTH

func _process(_delta):
	if Input.is_action_just_pressed("dbg_enemies_die"):
		enemy_health = 0
	if Input.is_action_just_pressed("dbg_enemies_take_damage"):
		enemy_take_damage(55)
	enemy_update_state()
	enemy_update_animation()

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
