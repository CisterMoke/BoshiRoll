extends RigidBody2D

@export var torque = 100
@onready var shape = $CollisionShape2D.shape

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


func _integrate_forces(state):
	if Input.is_action_pressed("right"):
		apply_torque(torque * 1.0/state.inverse_inertia)
	if  Input.is_action_pressed("left"):
		apply_torque(-torque * 1.0/state.inverse_inertia)
	if Input.is_action_just_pressed("up"):
		apply_impulse(Vector2(0, -500 * mass))
	if Input.is_action_pressed("down"):
		set_angular_velocity(0)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	if Input.is_action_just_pressed("respawn"):
		position = Vector2(0, 0)
		linear_velocity = Vector2(0, 0)
		angular_velocity = 0


func get_calculated_inertia():
	return 1.0/PhysicsServer2D.body_get_direct_state(get_rid()).inverse_inertia
