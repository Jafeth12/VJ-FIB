extends HSlider

@export var bus_name: String = "Master"
var bus_id: int

func _ready() -> void:
	bus_id = AudioServer.get_bus_index(bus_name)
	value_changed.connect(_on_value_changed)
	value = db_to_linear(AudioServer.get_bus_volume_db(bus_id))

func _on_value_changed(val: float) -> void:
	AudioServer.set_bus_volume_db(bus_id, linear_to_db(val))
