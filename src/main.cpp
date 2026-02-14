#include <iostream>
#include <chrono>
#include <thread>


#include "Log.h"
#include "Render.h"
#include "defines.h"
#include "GUI.h"
#include "HW_Con.h"
#include "Sequence_Handler.h"


int main()
{
	std::string log_origin = "MAIN";//Create such a string variable in every space needed with an name which tells, from where the logger was called
	Log logger("/logs/logs.txt");

	SDL_Window* window;

	

	//Initialize SDL
	if (SDL_InitSubSystem(NULL) < 0)//Return Value: 0 on success, negative value on failure
	{
		logger.writeLog(LogLevel::ERROR, log_origin + " SDL_INIT", SDL_GetError());
		std::cout << SDL_GetError() << std::endl;
	}

	int gui_state = 1; //used to iterate over the different GUIs
	int nmbr_of_guis = 3;//Ammount of used guis 

	//Initialize IMG
	//According to description, no return error check, because the function return the flag-values
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	if (TTF_Init() < 0)
	{
		logger.writeLog(LogLevel::ERROR, log_origin + " TTF_INIT", TTF_GetError());
	}

	//Create Window
	window = SDL_CreateWindow("BGA_Steuerung", 0, 0, 480, 800, SDL_WINDOW_FULLSCREEN);
	if (!window)
	{
		logger.writeLog(LogLevel::ERROR, log_origin + " SDL_CREATE_WINDOW", SDL_GetError());
		std::cout << SDL_GetError() << std::endl;
	}

	//Create a renderer, that is used by every GUI instance
	Render renderer(&logger, window);
	
	//Create GUI instance (main gui)
	GUI system_diagram(&logger, &renderer);

	GUI sequence_overview(&logger, &renderer);

	GUI io_overview(&logger, &renderer);

	//Create Hardware-handling class instance
	auto hw = std::make_shared<HW_Con>(&logger);

	hw->loadDigitalInputAdresses("/configs/D_In_config.txt");
	hw->loadAnalogInputAdresses("/configs/A_In_config.txt");
	hw->loadDigitalOutputAdresses("/configs/D_Out_config.txt");
	hw->loadIOMapConfig("/configs/HW_IO_map_config.txt");
	hw->initialisePCBRelayState();

	Sequence_Handler sqh(&logger, hw);
	


	sqh.loadSequences("/sequences");
	sqh.startAllSequences();
	
	

	system_diagram.loadFont("ARIAL_Black", "/fonts/arial.ttf", 0, 0, 0);
	system_diagram.loadTexture("Circular_Arrow", "/resource/circle_arrow.png");
	system_diagram.loadTexture("Schema", "/resource/schema_bga_v3.png");
	system_diagram.loadTexture("HALT_Symbol", "/resource/halt_symbol.png");
	system_diagram.loadTexture("Schieber_Geschlossen", "/resource/schieber_geschlossen.png");
	system_diagram.loadTexture("Schieber_Offen", "/resource/schieber_offen.png");
	system_diagram.loadTexture("Schieber_Unbekannt", "/resource/schieber_unbekannt.png");




	sequence_overview.loadFont("ARIAL_Black", "/fonts/arial.ttf", 0, 0, 0);
	sequence_overview.loadTexture("Seq_Liste", "/resource/Sequenzuebersicht.png");
	sequence_overview.loadTexture("Dreieck_Rechts", "/resource/dreieck_rechts.png");
	sequence_overview.loadTexture("Dreieck_Links", "/resource/dreieck_links.png");
	sequence_overview.loadTexture("Sequenz_Play_Pause", "/resource/play_pause.png");
	sequence_overview.loadTexture("Sequenz_Reload", "/resource/reload.png");

	io_overview.loadFont("ARIAL_Black", "/fonts/arial.ttf", 0, 0, 0);
	io_overview.loadTexture("IO_Liste", "/resource/schema_IO_v1.png");
	io_overview.loadTexture("IO_Aktiv", "/resource/stromkreis_geschlossen_gruen.png");
	io_overview.loadTexture("IO_Inaktiv", "/resource/stromkreis_offen_rot.png");


	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::chrono::system_clock::time_point last_frame = std::chrono::system_clock::now();
	std::chrono::duration<double, std::milli> total_time_last_frame;
	int target_fps = 30;

	auto time = std::chrono::system_clock::now();
	std::time_t current_time; 
	std::string str_current_time;


	

	//Main loop
	//{
	//	clear renderer
	//	draw things in backbuffer, from back to front
	//	show things
	//}

	
	
	int rotation = 0;
	int asked_sequence = 0;
	int scrollpos = 0;
	int y_corr = 0;
	bool io_state = false;

	do
	{

		now = std::chrono::system_clock::now();
		total_time_last_frame = now - last_frame;
		


		hw->refreshDigitalInputStates();


	
		
		

		if (total_time_last_frame.count() > static_cast<float>(1000) /target_fps)
		{	

			//if (!hw->getDigitalInputState("D_In_1"))
			//{
			//	hw->setDigitalOutputState(true, "D_Out_0");
			//}
			//else if (hw->getDigitalInputState("D_In_1"))
			//{
			//	hw->setDigitalOutputState(false, "D_Out_0");
			//}


			//Iterate over GUI state
			if (hw->getButton2Press())
			{
				gui_state++;
			
				if (gui_state > nmbr_of_guis - 1)
				{
					gui_state = 0;
				}
			}

		
			/*std::cout << "Button4: " << hw->getDigitalGPIOState(GPIO_BCM_BUTTON4) << std::endl;
			std::cout << "EncoderA: " << hw->getDigitalGPIOState(GPIO_BCM_ENCODER_A) << std::endl;
			std::cout << "EncoderB: " << hw->getDigitalGPIOState(GPIO_BCM_ENCODER_B) << std::endl;*/

			//Get system time
			time = now;
			current_time = std::chrono::system_clock::to_time_t(time);
			str_current_time = std::ctime(&current_time);
			//std::ctime inserts a newline at the end of the time data. Remove it with pop_back
			str_current_time.pop_back();

			renderer.clearRender();

				if (gui_state == 0)
				{


					if (hw->getEncoderPush())
					{
						system_diagram.switchCursorState();
					}
					system_diagram.prepareCursor();

					system_diagram.moveCursor(hw->getEncoderDirection());

				
					system_diagram.drawBackGroundColor(225, 225, 225);
					system_diagram.drawVisualElement(0, 0, 480, 800, "Schema", false);

					//Info Elements: Time and date, FPS, ms/frame
					system_diagram.drawVisualElement(0, 0, 480, 30, 245, 245, 245, false);
					system_diagram.drawText_l(231, 9, "FPS: " + std::to_string(target_fps), "ARIAL_Black");
					system_diagram.drawText_r(473, 9, "Time(ms)/frame: " + std::to_string((int)total_time_last_frame.count()), "ARIAL_Black");
					system_diagram.drawText_l(7, 9, str_current_time, "ARIAL_Black");
				

					system_diagram.drawVisualElement(78, 98, 20, 20, rotation, "Circular_Arrow", true);//Pumpe1
					system_diagram.drawVisualElement(78, 186, 20, 20, rotation, "Circular_Arrow", false);//Pumpe2
					system_diagram.drawVisualElement(111, 366, 20, 20, rotation, "Circular_Arrow", false);//Pumpe3
					system_diagram.drawVisualElement(213, 198, 20, 20, hw->getDoubleInputState("D_In_0", "D_In_1"), false);//Schieber_Rinderguelle
					system_diagram.drawVisualElement(261, 145, 20, 20, 270, "Schieber_Geschlossen", false);//Schieber_Mischer
					system_diagram.drawVisualElement(52, 716, 20, 20, "Schieber_Geschlossen", true);//Schieber_Probenahme
					system_diagram.drawVisualElement(172, 716, 20, 20, "Schieber_Geschlossen",false);//Schieber_Grube_Fankhauser
					system_diagram.drawVisualElement(291, 716, 20, 20, "Schieber_Geschlossen", false);//Schieber_Grube_Schlegel
					system_diagram.drawVisualElement(411, 716, 20, 20, "Schieber_Geschlossen", true);//Schieber_Fassbefuellung
					system_diagram.drawVisualElement(450, 659, 20, 20, "Schieber_Offen", false);//Schieber_Rueckfuehrung

					//system_diagram.drawPreloadedTexture(80, 186, 15, 15, rotation, "Arrow_Active");
					//system_diagram.drawText_l(50, 200, "Hello World", "ARIAL_Black");
					//system_diagram.drawText_l(80, 400, "Hello World", "ARIAL_Black");
					system_diagram.drawText_l(349, 112, std::to_string(4850) + "kg", "ARIAL_Black");
					system_diagram.drawText_l(349, 130, std::to_string(75) + "%", "ARIAL_Black");
				
				

					system_diagram.drawCursor();

				}

				//Sequence overview
				else if (gui_state == 1)
				{
					if (hw->getEncoderPush())
					{
						sequence_overview.switchCursorState();
					}
					sequence_overview.prepareCursor();
					
					if (hw->getButton1Press())
					{
						asked_sequence++;
						if (asked_sequence > sqh.getAmmountOfLoadedSequences() - 1)
						{
							asked_sequence = 0;
						}
					}

					sequence_overview.drawBackGroundColor(225, 225, 225);
					sequence_overview.drawVisualElement(0, 0, 480, 800, "Seq_Liste", false);


					//Info Elements: Time and date, FPS, ms/frame
					sequence_overview.drawVisualElement(0, 0, 480, 30, 245, 245, 245, false);
					sequence_overview.drawText_l(231, 9, "FPS: " + std::to_string(target_fps), "ARIAL_Black");
					sequence_overview.drawText_r(473, 9, "Time(ms)/frame: " + std::to_string((int)total_time_last_frame.count()), "ARIAL_Black");
					sequence_overview.drawText_l(7, 9, str_current_time, "ARIAL_Black");

					//Sequenzinfo
					sequence_overview.drawText_l(30, 105, sqh.getSequenceName(asked_sequence) + "  |  Position: " + std::to_string(sqh.getExecutionStep(sqh.getSequenceName(asked_sequence))) + " / Anzahl Schritte: " + std::to_string(sqh.getSequenceStepAmmount(asked_sequence)) + "", "ARIAL_Black");

					//Sequenzanzahl
					sequence_overview.drawText_l(367, 105, std::to_string(sqh.getAmmountOfLoadedSequences()), "ARIAL_Black");


					sequence_overview.drawList_l(23, 137, 22, 270, 648, hw->getButton3Press(), hw->getButton4Press(), sqh.getSequenceFunctions(asked_sequence), "ARIAL_Black");


					for (int i = 0; i < sqh.getAmmountOfLoadedSequences(); i++)
					{
						//Draw text for every loaded sequence. Offset the y-position for every sequence
						/*sequence_overview.drawText_l(305, 160 + i* 20, "Sequence " + std::to_string(i) + ": " + std::to_string(sqh.getExecutionStep(sqh.getSequenceName(i))), "ARIAL_Black");
						sequence_overview.drawText_l(305, 160 + i *20, "Sequence " + std::to_string(i) + ": " + std::to_string(sqh.getExecutionStep(sqh.getSequenceName(i))), "ARIAL_Black");*/

						sequence_overview.drawList_l(367, 160+ i*40, 20, { sqh.getSequenceName(i), std::to_string(sqh.getExecutionStep(sqh.getSequenceName(i)))}, "ARIAL_Black");
					}


					sequence_overview.drawCursor();
					
				}

				//io_overview
				else if (gui_state == 2)
				{
					if (hw->getEncoderPush())
					{
						io_overview.switchCursorState();
					}
					io_overview.prepareCursor();


					io_overview.drawBackGroundColor(225, 225, 225);
					io_overview.drawVisualElement(0, 0, 480, 800, "IO_Liste", false);

					//Info Elements: Time and date, FPS, ms/frame
					io_overview.drawVisualElement(0, 0, 480, 30, 245, 245, 245, false);
					io_overview.drawText_l(231, 9, "FPS: " + std::to_string(target_fps), "ARIAL_Black");
					io_overview.drawText_r(473, 9, "Time(ms)/frame: " + std::to_string((int)total_time_last_frame.count()), "ARIAL_Black");
					io_overview.drawText_l(7, 9, str_current_time, "ARIAL_Black");

					io_overview.drawText_l(10, 40, "Digitale Inputs: ", "ARIAL_Black");
					io_overview.drawText_l(300, 40, "Digitale Outputs: ", "ARIAL_Black");

					/*io_overview.drawVisualElement(295, 725, 100, 55, 200, 200, 200, false);
					io_overview.drawText_l(300, 730, "Rot = Inaktiv", "ARIAL_Black", 255, 0, 0);
					io_overview.drawText_l(300, 755, "Gruen = Aktiv", "ARIAL_Black", 0, 255, 0);*/

					
					
					

					for (int i = 0; i < 24; i++)
					{
						std::string input_name = "D_In_" + std::to_string(i);
						io_state = hw->getDigitalInputState(input_name);
						io_overview.drawVisualElement(120, 83 + y_corr, 80, 15, (io_state ? "IO_Inaktiv" : "IO_Aktiv"), false);
						y_corr += 30;

					}


					y_corr = 0;

					for (int i = 0; i < 16; i++)
					{
						std::string output_name = "D_Out_" + std::to_string(i);
						io_state = hw->getDigitalOutputState(output_name);
						io_overview.drawVisualElement(370, 83 + y_corr, 80, 15, (io_state ? "IO_Aktiv" : "IO_Inaktiv"), false);
						y_corr += 30;

					}
					
					y_corr = 0;

				}

			renderer.Show();

			//SDL_Delay(15000);

			rotation++;
			if (rotation > 360)
			{
				rotation = 0;
			}

			last_frame = now;
		}

		

	} while (true);



	logger.writeLog(LogLevel::INFO, log_origin, "SUCCESSFUL");

	return 0;
}