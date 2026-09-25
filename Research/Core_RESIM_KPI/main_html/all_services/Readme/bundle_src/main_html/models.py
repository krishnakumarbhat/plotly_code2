"""
Database Models for HPC Flask Application
Using SQLAlchemy ORM with PostgreSQL
"""
import os
from datetime import datetime
from flask_sqlalchemy import SQLAlchemy
from flask_login import UserMixin
from werkzeug.security import generate_password_hash, check_password_hash

db = SQLAlchemy()


class User(UserMixin, db.Model):
    """User model for authentication and authorization"""
    __tablename__ = 'users'
    
    id = db.Column(db.Integer, primary_key=True)
    net_id = db.Column(db.String(50), unique=True, nullable=False, index=True)
    name = db.Column(db.String(100), nullable=False)
    password_hash = db.Column(db.String(256), nullable=False)
    role = db.Column(db.String(20), default='user')  # 'user', 'admin'
    created_at = db.Column(db.DateTime, default=datetime.utcnow)
    last_login = db.Column(db.DateTime)
    is_active = db.Column(db.Boolean, default=True)
    
    # Relationship to job history
    jobs = db.relationship('JobHistory', backref='user', lazy='dynamic')
    chat_sessions = db.relationship('ChatSession', backref='user', lazy='dynamic')
    
    def set_password(self, password):
        """Hash and set user password"""
        self.password_hash = generate_password_hash(password)
    
    def check_password(self, password):
        """Verify password against hash"""
        return check_password_hash(self.password_hash, password)
    
    def update_last_login(self):
        """Update last login timestamp"""
        self.last_login = datetime.utcnow()
        db.session.commit()
    
    def __repr__(self):
        return f'<User {self.net_id}>'


class JobHistory(db.Model):
    """Job history tracking for all tool executions"""
    __tablename__ = 'job_history'
    
    id = db.Column(db.Integer, primary_key=True)
    user_id = db.Column(db.Integer, db.ForeignKey('users.id'), nullable=False)
    tool_name = db.Column(db.String(50), nullable=False, index=True)
    input_path = db.Column(db.String(500), nullable=False)
    input_filename = db.Column(db.String(255))  # Extracted HDF filename for display
    output_path = db.Column(db.String(500))
    slurm_job_id = db.Column(db.String(50), index=True)
    status = db.Column(db.String(20), default='QUEUED', index=True)
    # Status: QUEUED, RUNNING, COMPLETED, FAILED, CANCELLED
    output_log_path = db.Column(db.String(500))
    error_message = db.Column(db.Text)
    parameters = db.Column(db.JSON)  # Store additional parameters as JSON
    created_at = db.Column(db.DateTime, default=datetime.utcnow, index=True)
    started_at = db.Column(db.DateTime)
    completed_at = db.Column(db.DateTime)
    
    def get_duration(self):
        """Calculate job duration"""
        if self.started_at and self.completed_at:
            return (self.completed_at - self.started_at).total_seconds()
        return None
    
    def to_dict(self):
        """Convert to dictionary for API responses"""
        return {
            'id': self.id,
            'tool_name': self.tool_name,
            'input_filename': self.input_filename or self.input_path.split('/')[-1],
            'status': self.status,
            'slurm_job_id': self.slurm_job_id,
            'created_at': self.created_at.isoformat() if self.created_at else None,
            'completed_at': self.completed_at.isoformat() if self.completed_at else None,
            'duration': self.get_duration()
        }
    
    def __repr__(self):
        return f'<JobHistory {self.id} - {self.tool_name} - {self.status}>'


class ChatSession(db.Model):
    """Chat session history with LLM"""
    __tablename__ = 'chat_sessions'
    
    id = db.Column(db.Integer, primary_key=True)
    user_id = db.Column(db.Integer, db.ForeignKey('users.id'), nullable=False)
    session_id = db.Column(db.String(100), unique=True, nullable=False)
    title = db.Column(db.String(200), default='New Chat')
    created_at = db.Column(db.DateTime, default=datetime.utcnow)
    updated_at = db.Column(db.DateTime, default=datetime.utcnow, onupdate=datetime.utcnow)
    is_active = db.Column(db.Boolean, default=True)
    
    # Relationship to messages
    messages = db.relationship('ChatMessage', backref='session', lazy='dynamic')
    
    def __repr__(self):
        return f'<ChatSession {self.session_id}>'


class ChatMessage(db.Model):
    """Individual chat messages"""
    __tablename__ = 'chat_messages'
    
    id = db.Column(db.Integer, primary_key=True)
    session_id = db.Column(db.Integer, db.ForeignKey('chat_sessions.id'), nullable=False)
    role = db.Column(db.String(20), nullable=False)  # 'user', 'assistant'
    content = db.Column(db.Text, nullable=False)
    created_at = db.Column(db.DateTime, default=datetime.utcnow)
    
    def __repr__(self):
        return f'<ChatMessage {self.id} - {self.role}>'


def init_db(app):
    """Initialize the database"""
    with app.app_context():
        db.create_all()

        disable_default_admin = (os.environ.get('HPC_TOOLS_DISABLE_DEFAULT_ADMIN') or '').strip().lower() in {'1', 'true', 'yes', 'y', 'on'}
        auth_mode = (os.environ.get('HPC_TOOLS_AUTH_MODE') or '').strip().lower()
        disable_default_admin = disable_default_admin or auth_mode == 'cluster'

        default_admin = User.query.filter_by(net_id='admin').first()
        if disable_default_admin:
            if default_admin and default_admin.is_active:
                default_admin.is_active = False
                db.session.commit()
                print('Disabled default admin user for cluster-auth deployment')
            return

        # Create default admin user if not exists
        if not default_admin:
            admin = User(
                net_id='admin',
                name='Administrator',
                role='admin'
            )
            admin.set_password('admin123')
            db.session.add(admin)
            db.session.commit()
            print("Created default admin user (net_id: admin, password: admin123)")
