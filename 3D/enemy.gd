class_name GenericEnemy extends GenericEntity

# Estado del enemigo
enum EnemyState { WAIT, WANDER, ATTACK, DEAD }
var enemy_state: EnemyState = EnemyState.WAIT

func _process(_delta):
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
# Use this function to see if the enemy should die
# p.e.: if it's life is empty
func enemy_should_die() -> bool:
	return false

# VIRTUAL. TO BE OVERRIDEN
# Use this function to indicate that the enemy has finished
# its attack
# p.e.: when the attack animation is finished
func enemy_is_attack_finished() -> bool:
	return false
