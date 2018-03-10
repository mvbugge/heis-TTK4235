#include "orderSys.h"


void order_update(elev_button_type_t button, int floor) {
	active_buttons[floor][button] = 1;

	/* add new order to queue */
	if (button == BUTTON_COMMAND) {
		for (int i = 0; i<4; i++) {
			if (queue[i] == floor || queue[i] == -1) {
				queue[i] = floor;
				break;
			}
		}
	}
}


void order_remove(int floor) {

	for (int i = 0; i<3; i++) {
		active_buttons[floor][i] = 0;
	}

	/* Moves orders forward in queue*/
	for (int i = 0; i<4; i++) {
		if (queue[i] == floor) {
			if (i != 3) {
				for (int j = i; j < 3; j++) {
					queue[j] = queue[j + 1];
				}
			};
			queue[3] = -1;
		};
	}
}


void order_clear_all() {
	for (int i = 0; i<4; i++) {
		queue[i] = -1;
		for (int j = 0; j<3; j++) {
			active_buttons[i][j] = 0;
		}
	}

}


elev_motor_direction_t order_get_dir(int floor) {

	if (queue[0]<floor && queue[0]!=-1) {
		return DIRN_DOWN;
	};
	if (queue[0]>floor && queue[0]!=-1) {
		return DIRN_UP;
	};

	if (queue[0] == floor && queue[0]!=-1) {
		return DIRN_STOP;
	};

	int requests_up = 0;		//above
	int requests_down = 0;
	for (int f = 0; f<floor; f++) {
		requests_down += active_buttons[f][1];
		requests_down += active_buttons[f][0];
	}
	for (int f = floor+1; f < 4; f++){
		requests_up += active_buttons[f][1];
		requests_up += active_buttons[f][0];
	}
	if (requests_up >= requests_down && requests_up != 0) {
		return DIRN_UP;
	};
	if (requests_down>requests_up) {
		return DIRN_DOWN;
	};
	return DIRN_STOP;

}

elev_motor_direction_t order_get_dir_d(double floor) {

	if (queue[0]<floor && queue[0]!=-1) {
		return DIRN_DOWN;
	};
	if (queue[0]>floor && queue[0]!=-1) {
		return DIRN_UP;
	};

	if (queue[0] == floor && queue[0]!=-1) {
		return DIRN_STOP;
	};

	int requests_up = 0;		//above
	int requests_down = 0;
	for (int f = 0; f<floor; f++) {
		requests_down += active_buttons[f][1];
		requests_down += active_buttons[f][0];
	}
	for (int f = floor+1; f < 4; f++){
		requests_up += active_buttons[f][1];
		requests_up += active_buttons[f][0];
	}
	if (requests_up >= requests_down && requests_up != 0) {
		return DIRN_UP;
	};
	if (requests_down>requests_up) {
		return DIRN_DOWN;
	};
	return DIRN_STOP;

}


bool order_should_stop(int floor, elev_motor_direction_t dir) {
	
	/* Check if the current floor is in queue */
	for (int i = 0; i<4; i++) {
		if (queue[i] == floor) {
			return 1;
		}
	}
	/* Check if the current floor is requested and in same direction */
	if (dir == -1 && active_buttons[floor][1]) {
		return 1;
	}
	if (dir == 1 && active_buttons[floor][0]) {
		return 1;
	}
	
	if (order_no_request_current_dir(dir) 
		&& (active_buttons[floor][0] 
		|| active_buttons[floor][1])){
		return 1;
	}

	return 0;

}


bool orders_finished() {
	for (int i = 0; i < 4; i++) {
		if (queue[i] != -1 
			|| active_buttons[i][0] != 0 
			|| active_buttons[i][1] != 0 
			||  active_buttons[i][2] != 0) {
			return 0;
		}

	}
	return 1;
}


/* returns 1 if no request in current direction */
bool order_no_request_current_dir(elev_motor_direction_t dir){
	int button;

	if(dir == -1){
		button = BUTTON_CALL_DOWN;
	}
	if(dir == 1){
		button = BUTTON_CALL_UP;
	}
	for (int i = 0; i < 4; i++) {
		if (active_buttons[button][i]) {
			return 0;
		}

	}
	return 1;
}