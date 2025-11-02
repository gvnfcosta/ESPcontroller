// build.gradle.kts (PROJECT LEVEL – RAIZ)

import org.gradle.api.tasks.Delete
import org.gradle.api.file.Directory

// ---------------------------------------------------------------
// 1. MUDANÇA DO DIRETÓRIO DE BUILD (OPCIONAL)
// ---------------------------------------------------------------
val newBuildDir: Directory = rootProject.layout.buildDirectory
    .dir("../../build")
    .get()

rootProject.layout.buildDirectory.value(newBuildDir)

subprojects {
    val subprojectBuildDir = newBuildDir.dir(project.name)
    project.layout.buildDirectory.value(subprojectBuildDir)
}

// ---------------------------------------------------------------
// 2. TAREFA CLEAN PERSONALIZADA
// ---------------------------------------------------------------
tasks.register<Delete>("clean") {
    delete(rootProject.layout.buildDirectory)
}

// ---------------------------------------------------------------
// 3. DECLARAÇÃO DE PLUGINS (apenas declaração – apply false)
// ---------------------------------------------------------------
plugins {
    id("com.android.application") version "8.9.1" apply false
    id("org.jetbrains.kotlin.android") version "2.1.0" apply false   // versão compatível com AGP 8.9.x
    id("com.google.gms.google-services") version "4.4.2" apply false
}