class_name Crawler extends GenericEnemy

func get_new_alpha(delta: float) -> float:
	var dir = 1
	match state:
		EnemyState.WANDER:
			return alpha + dir * SPEED * delta
	return alpha

