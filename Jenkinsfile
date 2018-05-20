pipeline {
	agent any
	
	stages {
		stage('Build') {
			steps {
				cleanWs()
				
				git url: 'git://github.com/GuiTeK/transporter.git', branch: 'master'
				
				sh '''
					mkdir build
					cd build
					cmake ..
					make
				'''
			}
		}
		
		stage('Test') {
			steps {
				sh '''
					cd build
					./transporter.tests/transporter.tests
				'''
			}
		}
	}
}
