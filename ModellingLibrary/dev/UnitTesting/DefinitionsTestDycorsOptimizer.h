
#define TEST_DYCORS_INITIAL_STEP_SIZE 0.99
#define TEST_DYCORS_MIN_STEP_SIZE 0.001
#define TEST_DYCORS_NUMBER_OF_NEIGHBOURS 10
#define TEST_DYCORS_CONSECUTIVE_FAIL_TOLERANCE 5
#define TEST_DYCORS_CONSECUTIVE_SUCCESS_TOLERANCE 5
#define TEST_DYCORS_RBF_WEIGHT 0.4

#define TEST_DYCORS_SPHERE_FUNCTION_NUMBER_OF_EVALUATIONS 70
#define TEST_DYCORS_SPHERE_FUNCTION_OBJECTIVE_NAME "Objective1"
#define TEST_DYCORS_SPHERE_FUNCTION_INPUT_NAMES {"Input1", "Input2"}

#define TEST_DYCORS_ZDT1_FUNCTION_PATH_TO_SOLUTION "../UnitTesting/TestSolutionFiles/dycorsZdt1TestParetoFront.csv"
#define TEST_DYCORS_ZDT1_FUNCTION_NUMBER_OF_EVALUATIONS 200
#define TEST_DYCORS_ZDT1_FUNCTION_OBJECTIVE1_NAME "Objective1"
#define TEST_DYCORS_ZDT1_FUNCTION_OBJECTIVE2_NAME "Objective2"
#define TEST_DYCORS_ZDT1_FUNCTION_INPUT_NAMES {"Input1", "Input2", "Input3", "Input4", "Input5", "Input6", \
                                               "Input7", "Input8", "Input9", "Input10", "Input11", \
                                               "Input12", "Input13", "Input14", "Input15", "Input16", \
                                               "Input17", "Input18", "Input19", "Input20", \
                                               "Input21", "Input22", "Input23", "Input24", \
                                               "Input25", "Input26", "Input27", \
                                               "Input28", "Input29", "Input30"}

#define MONEY_VS_TIME_FUNCTION_OBJECTIVE1_NAME "Total time"
#define MONEY_VS_TIME_FUNCTION_OBJECTIVE2_NAME "Total money earned"
#define MONEY_VS_TIME_FUNCTION_INPUT_NAME_PREFIX1 "Time-increment"
#define MONEY_VS_TIME_FUNCTION_INPUT_NAME_PREFIX2 "Knowledge-increment"
#define MONEY_VS_TIME_FUNCTION_MAX_NUMBER_OF_TIME_STEPS 10
#define MONEY_VS_TIME_FUNCTION_KNOWLEDGE_WEIGHT 10
#define MONEY_VS_TIME_FUNCTION_INITIAL_MONEY 100
#define MONEY_VS_TIME_FUNCTION_INITIAL_KNOWLEDGE 0
#define MONEY_VS_TIME_FUNCTION_NUMBER_OF_EVALUATIONS 300
#define MONEY_VS_TIME_FUNCTION_PATH_TO_SOLUTION "../UnitTesting/TestSolutionFiles/moneyVsTimeTestParetoFront.csv"


