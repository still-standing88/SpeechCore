using System;
using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;
using VideoEditor.Core.Domain.Entities;
using VideoEditor.Core.Domain.ValueObjects;

namespace VideoEditor.Core.Application.Services
{
    /// <summary>
    /// Service for managing projects
    /// </summary>
    public class ProjectService
    {
        private Project? _currentProject;
        private readonly List<Project> _recentProjects = new();

        public event EventHandler<ProjectChangedEventArgs>? ProjectChanged;
        public event EventHandler<ProjectSavedEventArgs>? ProjectSaved;

        public Project? CurrentProject => _currentProject;
        public IReadOnlyList<Project> RecentProjects => _recentProjects.AsReadOnly();

        public Project CreateNew(string name, ProjectSettings? settings = null)
        {
            var project = new Project(name, settings);
            SetCurrentProject(project);
            return project;
        }

        public async Task<Project?> LoadAsync(string filePath)
        {
            try
            {
                if (!File.Exists(filePath))
                    throw new FileNotFoundException($"Project file not found: {filePath}");

                // TODO: Implement project file deserialization
                // For now, create a mock project
                var project = new Project(Path.GetFileNameWithoutExtension(filePath))
                {
                    FilePath = filePath
                };

                SetCurrentProject(project);
                AddToRecentProjects(project);
                
                return project;
            }
            catch (Exception ex)
            {
                throw new InvalidOperationException($"Failed to load project: {ex.Message}", ex);
            }
        }

        public async Task<bool> SaveAsync(Project? project = null)
        {
            project ??= _currentProject;
            if (project == null)
                throw new InvalidOperationException("No project to save");

            try
            {
                if (string.IsNullOrEmpty(project.FilePath))
                    throw new InvalidOperationException("Project file path not set");

                // TODO: Implement project file serialization
                project.MarkAsModified();
                
                ProjectSaved?.Invoke(this, new ProjectSavedEventArgs(project));
                return true;
            }
            catch (Exception ex)
            {
                throw new InvalidOperationException($"Failed to save project: {ex.Message}", ex);
            }
        }

        public async Task<bool> SaveAsAsync(string filePath, Project? project = null)
        {
            project ??= _currentProject;
            if (project == null)
                throw new InvalidOperationException("No project to save");

            try
            {
                project.FilePath = filePath;
                project.Name = Path.GetFileNameWithoutExtension(filePath);
                
                return await SaveAsync(project);
            }
            catch (Exception ex)
            {
                throw new InvalidOperationException($"Failed to save project as: {ex.Message}", ex);
            }
        }

        public void CloseProject()
        {
            if (_currentProject != null)
            {
                var previousProject = _currentProject;
                _currentProject = null;
                ProjectChanged?.Invoke(this, new ProjectChangedEventArgs(previousProject, null));
            }
        }

        public bool IsProjectModified(Project? project = null)
        {
            project ??= _currentProject;
            return project != null && project.LastModified > DateTime.MinValue;
        }

        private void SetCurrentProject(Project project)
        {
            var previousProject = _currentProject;
            _currentProject = project;
            ProjectChanged?.Invoke(this, new ProjectChangedEventArgs(previousProject, project));
        }

        private void AddToRecentProjects(Project project)
        {
            _recentProjects.RemoveAll(p => p.FilePath == project.FilePath);
            _recentProjects.Insert(0, project);
            
            // Keep only the 10 most recent projects
            if (_recentProjects.Count > 10)
                _recentProjects.RemoveRange(10, _recentProjects.Count - 10);
        }
    }

    /// <summary>
    /// Event arguments for project changes
    /// </summary>
    public class ProjectChangedEventArgs : EventArgs
    {
        public Project? PreviousProject { get; }
        public Project? NewProject { get; }

        public ProjectChangedEventArgs(Project? previousProject, Project? newProject)
        {
            PreviousProject = previousProject;
            NewProject = newProject;
        }
    }

    /// <summary>
    /// Event arguments for project saved events
    /// </summary>
    public class ProjectSavedEventArgs : EventArgs
    {
        public Project Project { get; }

        public ProjectSavedEventArgs(Project project)
        {
            Project = project ?? throw new ArgumentNullException(nameof(project));
        }
    }
}