#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"

#include "aep-core/shader.h"
#include "aep-core/gl_aux.h"
#include "aep-core/AEPFilter.h"
#include "aep-core/AEPComposer.h"

using namespace std;


// settings
static unsigned int SCR_WIDTH = 800;
static unsigned int SCR_HEIGHT = 600;

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static CShader* createShader_sample();
static void processInput(GLFWwindow* window);
static unsigned int makeVAO();

int demo_filter()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ImageTexture* pIT = createImageTexture((char*)"resources/7-enh-qua.jpg");
	if (pIT == NULL)
		return -1;

	CShader* pShader = createShader_sample();
	unsigned int texture = createTexture_from_image((char*)"resources/7-enh-qua.jpg");
	unsigned int VAO = makeVAO();

	pShader->use();
	pShader->setInt("texture1", 0);

	unsigned int textureColorMap = createTexture_from_image((char*)"resources/filterColor/industry.png");
	unsigned int textureTarget = createTexture_from_image((char*)"resources/he-base.jpg");
	CAEPFilter* pEffect = new CAEPFilter((char*)"./shader/filter-0-copy.vs", (char*)"./shader/filter-0-copy.fs");
	//CAEPFilter* pEffect = new CAEPFilter((char*)"./shader/transition_turnpage.vs", (char*)"./shader/transition_turnpage.fs", FM_Transition);
	if (pEffect->Open(pIT->_width, pIT->_height) != 0)
		return -1;
	//pEffect->Set_ColorMap(textureColorMap);
	pEffect->Set_TransitionTarget(textureTarget);

	// render loop
	// -----------
	int count1 = 0;
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

#if 1
		float fProcess = float(count1%1000) / 1000;
		//fProcess = count1*10;
		//fProcess = 0.01 * count1;
		count1++;
		pEffect->Render(fProcess, pIT->_textureID);
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		
		static int count = 0;
		//cout << count++ << endl;
		//#else
				// render
				// ------
		glClearColor(0.9f, 0.9f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// bind Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, pEffect->Get_textureOut());
		//glBindTexture(GL_TEXTURE_2D, texture);

		// render container
		pShader->use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
#endif
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();

	return 0;
}

void set_desc_default(LayerDesc* pDesc, int index)
{
	pDesc->_nStartTime_ms = 0;
	pDesc->_nEndTime_ms = 100 * 1000;
	pDesc->_fStartRotateAngle = 0;
	pDesc->_fEndRotateAngle = 360*2;
	pDesc->_vecStartPos[0].x = 0.0;
	pDesc->_vecStartPos[0].y = 0.0;
	pDesc->_vecStartPos[1].x = 1.0;
	pDesc->_vecStartPos[1].y = 1.0;
	if (index == 0)
	{
		pDesc->_vecEndPos[0].x = 0.5;
		pDesc->_vecEndPos[0].y = 0.5;
		pDesc->_vecEndPos[1].x = 1.0;
		pDesc->_vecEndPos[1].y = 1.0;
		pDesc->_fStartAlpha = 0;
		pDesc->_fEndAlpha = 1.0;
	}
	else
	{
		pDesc->_vecEndPos[0].x = 0.1;
		pDesc->_vecEndPos[0].y = 0.1;
		pDesc->_vecEndPos[1].x = 0.3;
		pDesc->_vecEndPos[1].y = 0.3;
		pDesc->_fStartAlpha = 1.0;
		pDesc->_fEndAlpha = 0.0;
	}
	
	pDesc->_szImageName = (char *)"resources/he-base.jpg";
	pDesc->_eEffectType = ET_TRANSITION;
	//pDesc->_eEffectType = ET_NONE;
	pDesc->_szTransitionTarget = (char*)"resources/7-enh-qua.jpg";
}

int demo_composer()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "COMPOSER", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ImageTexture* pIT = createImageTexture((char*)"resources/7-enh-qua.jpg");
	if (pIT == NULL)
		return -1;

	CShader* pShader = createShader_sample();
	unsigned int texture = createTexture_from_image((char*)"resources/7-enh-qua.jpg");
	unsigned int VAO = makeVAO();

	pShader->use();
	pShader->setInt("texture1", 0);

	CAEPComposer* pComposer = new CAEPComposer();
	pComposer->Open(SCR_WIDTH, SCR_HEIGHT);

	for (int i = 0; i < 2; i++)
	{
		CAEPLayer* pLayer = new CAEPLayer(pComposer);
		LayerDesc desc;
		set_desc_default(&desc, i);
		pLayer->Open(&desc);
		pComposer->Add_Layer(pLayer);
	}

	// render loop
	// -----------
	int count1 = 0;
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

#if 1
		float fProcess = float(count1 % 1000) / 1000;
		//fProcess = count1*10;
		//fProcess = 0.01 * count1;
		count1++;
		pComposer->Render(count1*4);
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

		static int count = 0;
		//cout << count++ << endl;
		//#else
				// render
				// ------
		glClearColor(0.9f, 0.9f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// bind Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, pComposer->Get_textureOut());
		//glBindTexture(GL_TEXTURE_2D, texture);

		// render container
		pShader->use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
#endif
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();

	return 0;
}

int main(int argc, char* argv[])
{
	cout << "hi, aep render man!" << endl;

	//demo_filter();
	demo_composer();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	SCR_WIDTH = width;
	SCR_HEIGHT = height;

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
}

CShader* createShader_sample()
{
	CShader* pShader = new CShader("./shader/filter-0-copy.vs", "./shader/filter-0-copy.fs");
	if (pShader == NULL)
	{
		std::cout << "new shader fail\n";
		return NULL;
	}
	return pShader;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


unsigned int makeVAO()
{
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions          // colors           // texture coords
		 1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	return VAO;
}
