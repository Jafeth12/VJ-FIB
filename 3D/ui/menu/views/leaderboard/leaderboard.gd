extends Control

var scores: Array = []

# Called when the node enters the scene tree for the first time.
func _ready():
	scores = MainLogic.get_scores()
	draw_scores()
	
func draw_scores():
	var list = $Panel/list
	
	for i in list.get_child_count():
		if get_child(i) != null:
			list.get_child(i).queue_free()
	
	for entry in scores:
		var text = Label.new()
		text.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
		text.text = MainLogic.format_time(entry) # str(snapped(entry, 0.01)) + " seconds"
			
		list.add_child(text)

func _on_button_back():
	SceneTransitions.change_scene("res://ui/menu/menu.tscn")
