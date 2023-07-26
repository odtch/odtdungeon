#include "stdodt.h"
#include "CharImporter.h"
#include "CharCharacterType.h"
#include "CharRagdoll.h"
#include "CharJoint.h"
#include "CharSkin.h"
#include "CharAnimation.h"
#include "converter/AssImp.h"
#include "converter/AssImpAnimation.h"
#include "converter/AssImpUtils.h"
#include "skin/SkinImporter.h"
#include "skin/SkinMesh.h"
#include "resource/MeshBuilder.h"

/*}

CharAnimationImporter::CharAnimationImporter( const QVector3D& render_offset, CharAnimationSystem* animations, CharImporter* character_importer, const QList<CharCharacter*>& test_characters )
	:_render_offset( render_offset )
	,_animations( animations )
	,_character_importer( character_importer )
	,_test_characters( test_characters )
	,_time_to_next_step( 0 )
	,_skeleton_animation( null )
	,_step_in_animation( 0 )
	,_animation_skeleton_in_step( null )
	,_current_sequence( null )
	,_animation_skeleton( null )
	,_lines( 1000000 )
{
	_left_foot.imp = &character_importer->_l.foot;
	_right_foot.imp = &character_importer->_r.foot;
}
CharAnimationImporter::~CharAnimationImporter(){
	delete _animation_skeleton;
	delete _skeleton_animation;
}

CharCharacter* _posed_character;
void CharAnimationImporter::start(){

	//importSequence( &_animations->_walk_f, "/home/rt/media/mocap/FBX_Ninja_v27_Pro/Animation/Root_Motion/NJA_Rlx_Walk_Forward_Loop.fbx" );
	importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/IPC/MOB1_Stand_Relaxed_Idle_v2_IPC.fbx" );

//	importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/IPC/MOB1_Walk_F_Loop_IPC.fbx" );
//	importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/IPC/MOB1_Walk_B_Loop_IPC.fbx" );
//	importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/IPC/MOB1_Walk_L_Loop_IPC.fbx" );
//	importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/IPC/MOB1_Walk_R_Loop_IPC.fbx" );
//	importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/IPC/MOB1_Stand_Rlx_Turn_In_Place_L_Loop_IPC.fbx" );
//	importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/IPC/MOB1_Stand_Rlx_Turn_In_Place_R_Loop_IPC.fbx" );

//	importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Run_F_Loop.fbx" );
//	importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Walk_F_Loop.fbx" );
//	importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Walk_B_Loop.fbx" );
//	importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Walk_L_Loop.fbx" );




//	importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Ninja_v27_Pro/Animation/IPC/NJA_Rlx_Stand_Idle_IPC.fbx" );

//	importSequence( &_animations->_walk_f, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/IPC/Split_Jumps/MOB1_Run_F_Jump_LU_Start_IPC.fbx" );
//			importSequence( &_animations->_walk_f, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/IPC/Split_Jumps/MOB1_Run_F_Jump_RU_Start_IPC.fbx" );
//			importSequence( &_animations->_walk_f, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/IPC/Split_Jumps/MOB1_Walk_L_Jump_LU_Start_IPC.fbx" );

//			importSequence( &_animations->_walk_f, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/IPC/MOB1_CrouchWalk_F_Loop_IPC.fbx" );
//			importSequence( &_animations->_walk_f, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/IPC/MOB1_Run_F_Loop_IPC.fbx" );
//			importSequence( &_animations->_walk_f, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/IPC/MOB1_Run_L_CIR_Loop_IPC.fbx" );


//	importSequence( &_animations->_walk_f, "/home/rt/media/mocap/FBX_Ninja_v27_Pro/Animation/IPC/NJA_Rlx_Walk_Forward_Loop_IPC.fbx" );
//	importSequence( &_animations->_walk_f, "/home/rt/media/mocap/FBX_Ninja_v27_Pro/Animation/IPC/NJA_Rlx_Jog_Forward_Loop_IPC.fbx" );
//	importSequence( &_animations->_walk_f, "/home/rt/media/mocap/FBX_Ninja_v27_Pro/Animation/IPC/NJA_Rlx_Run_Forward_Loop_IPC.fbx" );

//	importSequence( &_animations->_walk_f, "/home/rt/media/mocap/FBX_Ninja_v27_Pro/Animation/IPC/NJA_Rlx_Stand_L_90_IPC.fbx" );
//			importSequence( &_animations->_walk_f, "/home/rt/media/mocap/FBX_Ninja_v27_Pro/Animation/IPC/NJA_Rlx_Walk_Left_Loop_IPC.fbx" );
//	importSequence( &_animations->_walk_f, "/home/rt/media/mocap/FBX_Ninja_v27_Pro/Animation/IPC/NJA_Rlx_Jog_Left_Loop_IPC.fbx" );
//			importSequence( &_animations->_walk_f, "/home/rt/media/mocap/FBX_Ninja_v27_Pro/Animation/IPC/NJA_Rlx_Run_Left_Loop_IPC.fbx" );

//			importSequence( &_animations->_walk_f, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/IPC/Split_Jumps/MOB1_Jog_F_Jump_LU_Start_IPC.fbx" );
//					importSequence( &_animations->_walk_f, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/IPC/Split_Jumps/MOB1_Jog_F_Jump_RU_Start_IPC.fbx" );


//			importSequence( &_animations->_walk_f, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/IPC/MOB1_Walk_F_Loop_IPC.fbx" );
//					importSequence( &_animations->_walk_f, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/IPC/MOB1_Walk_FL_Loop_IPC.fbx" );
//					importSequence( &_animations->_walk_f, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/IPC/MOB1_Walk_L_Loop_IPC.fbx" );

//			importSequence( &_animations->_walk_f, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/IPC/MOB1_CrouchWalk_F_Loop_IPC.fbx" );

//			importSequence( &_animations->_walk_f, "/home/rt/media/mocap/FBX_Ninja_v27_Pro/Animation/IPC/NJA_Rlx_Stand_Jump_Up_Hang_Ptfm_Hi_IPC.fbx" );
//	importSequence( &_animations->_walk_f, "/home/rt/media/mocap/FBX_Ninja_v27_Pro/Animation/IPC/NJA_Kick_Back_v1_IPC.fbx" );
//	importSequence( &_animations->_walk_f, "/home/rt/media/mocap/FBX_Ninja_v27_Pro/Animation/IPC/NJA_Hang_Ptfm_Hi_Drop_To_Rlx_Stand_IPC.fbx" );
//			importSequence( &_animations->_walk_f, "/home/rt/media/mocap/FBX_Ninja_v27_Pro/Animation/IPC/NJA_Run_Flip_Run_IPC.fbx" );


////	CharAnimationSequence* run_to_run = importSequence( &_animations->_run_f, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/IPC/MOB1_Run_F_Loop_IPC.fbx" );
////	CharAnimationSequence* run_to_stand = importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/IPC/MOB1_Run_F_To_Stand_Relaxed_IPC.fbx" );
////	CharAnimationSequence* run_l_to_stand = importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/IPC/MOB1_Run_F_To_Stand_Relaxed_LU_IPC.fbx" );
////	CharAnimationSequence* run_r_to_stand = importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/IPC/MOB1_Run_F_To_Stand_Relaxed_RU_IPC.fbx" );


//importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Stand_Relaxed_To_Walk_L45_Fwd.fbx" );
//importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Stand_Relaxed_To_Walk_R.fbx" );
//importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Stand_Relaxed_To_Walk_R180_Fwd.fbx" );
//importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Walk_FR_Loop.fbx" );
//importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Walk_R_90.fbx" );

//importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Run_F_Jump.fbx" );
//importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Run_F_Jump_LU.fbx" );
//		importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Run_F_Jump_RU.fbx" );
//		importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Run_FL_Loop.fbx" );
//		importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Run_FR_Loop.fbx" );
//		importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Run_L_90.fbx" );
//		importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Run_L_180.fbx" );
//		importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Run_L_CIR_Loop.fbx" );
//		importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Run_L_Jump_LU.fbx" );
//		importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Run_L_Jump_RU.fbx" );
//		importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Run_L_Loop.fbx" );


//	CharAnimationSequence* stand_to_stand = importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Stand_Relaxed_Idle_v2.fbx" );
//	CharAnimationSequence* stand_to_walk = importSequence( &_animations->_walk_f, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Stand_Relaxed_To_Walk_F.fbx" );
//	CharAnimationSequence* walk_to_walk = importSequence( &_animations->_walk_f, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Walk_F_Loop.fbx" );
//	CharAnimationSequence* walk_to_stand = importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Walk_F_To_Stand_Relaxed.fbx" );
//	CharAnimationSequence* walk_l_to_stand = importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Walk_F_To_Stand_Relaxed_LU.fbx" );
//	CharAnimationSequence* walk_r_to_stand = importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Walk_F_To_Stand_Relaxed_RU.fbx" );
//	CharAnimationSequence* stand_to_crouch = importSequence( &_animations->_crouch, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Stand_Relaxed_To_Crouch.fbx" );
//	CharAnimationSequence* crouch_to_crouch = importSequence( &_animations->_crouch, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Crouch_Idle_V2.fbx" );
//	CharAnimationSequence* crouch_to_stand = importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Crouch_To_Stand_Relaxed.fbx" );
//	CharAnimationSequence* stand_to_run = importSequence( &_animations->_run_f, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Stand_Relaxed_To_Run_F.fbx" );
//	CharAnimationSequence* run_to_run = importSequence( &_animations->_run_f, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Run_F_Loop.fbx" );
//	CharAnimationSequence* run_to_stand = importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Run_F_To_Stand_Relaxed.fbx" );
//	CharAnimationSequence* run_l_to_stand = importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Run_F_To_Stand_Relaxed_LU.fbx" );
//	CharAnimationSequence* run_r_to_stand = importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Run_F_To_Stand_Relaxed_RU.fbx" );

//	CharAnimationSequence* jog_to_jog = importSequence( &_animations->_run_f, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Jog_F_Loop.fbx" );
//	CharAnimationSequence* jog_to_stand = importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Jog_F_To_Stand_Relaxed.fbx" );
//	CharAnimationSequence* jog_l_to_stand = importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Jog_F_To_Stand_Relaxed_LU.fbx" );
//	CharAnimationSequence* jog_r_to_stand = importSequence( &_animations->_stand, "/home/rt/media/mocap/FBX_Mobility_27B_Pro/Animation/Root_Motion/MOB1_Jog_F_To_Stand_Relaxed_RU.fbx" );

//	map_all_to_start( stand_to_stand, stand_to_walk );
//	map_last_to_start( stand_to_walk, walk_to_walk );
//	map_last_to_start( walk_to_walk, walk_to_walk );

////	map_one( walk_to_walk, 0, &_animations->_stand, walk_to_stand, 1 );
////	map_one( walk_to_stand, 39, &_animations->_stand, stand_to_stand, 0 );
//	map_one( walk_to_walk, 16, &_animations->_stand, walk_l_to_stand, 0 );
//	map_one( walk_l_to_stand, 23, &_animations->_stand, stand_to_stand, 0 );
//	map_one( walk_to_walk, 23, &_animations->_stand, walk_r_to_stand, 1 );
//	map_one( walk_r_to_stand, 23, &_animations->_stand, stand_to_stand, 0 );


//	_posed_character = new CharCharacter( _character_importer->_character_type );
////	map_best_best( "stand_to_walk", stand_to_stand, stand_to_walk );
////	map_best_best( "walk_to_walk", walk_to_walk, walk_to_walk );
//	//map_best_best( "walk_l_to_stand", &_animations->_stand, walk_to_walk, walk_l_to_stand );
//	//map_best_best( "walk_r_to_stand", &_animations->_stand, walk_to_walk, walk_r_to_stand );
//	map_best_best( "walk_to_stand", &_animations->_stand, walk_to_walk, walk_to_stand );
//	map_best_best( "walk_to_standl", &_animations->_stand, walk_to_stand, walk_l_to_stand );
//	map_best_best( "walk_to_standr", &_animations->_stand, walk_to_stand, walk_r_to_stand );

//	//map_best_best( "stand_to_run", stand_to_stand, stand_to_run );
//	//map_best_best( "run_to_run", run_to_run, run_to_run );
//	//map_best_best( "run_l_to_stand", &_animations->_stand, run_to_run, run_l_to_stand );
//	map_best_best( "run_to_stand", &_animations->_stand, run_to_run, run_to_stand );
//	//map_best_best( "run_r_to_stand", &_animations->_stand, run_to_run, run_r_to_stand );


//	map_last_to_start( stand_to_stand, stand_to_stand );

//	//map_last_to_start( walk_to_walk, walk_l_to_stand );
//	map_last_to_start( walk_to_stand, stand_to_stand );
//	map_last_to_start( walk_l_to_stand, stand_to_stand );
//	map_last_to_start( walk_r_to_stand, stand_to_stand );

//	map_all_to_start( stand_to_stand, stand_to_run );
//	map_last_to_start( stand_to_run, run_to_run );
//	map_last_to_start( run_to_run, run_to_run );
//	//map_last_to_start( run_to_run, run_l_to_stand );
//	map_last_to_start( run_to_stand, stand_to_stand );
//	map_last_to_start( run_l_to_stand, stand_to_stand );
//	map_last_to_start( run_r_to_stand, stand_to_stand );

//	map_all_to_start( stand_to_stand, stand_to_crouch );
//	map_last_to_start( stand_to_crouch, crouch_to_crouch );
//	map_last_to_start( crouch_to_crouch, crouch_to_crouch );
//	map_all_to_start( crouch_to_crouch, crouch_to_stand );
//	map_last_to_start( crouch_to_stand, stand_to_stand );

	_pending_animation_filenames = _all_animation_filenames;
	_time_to_next_step = 0;
	_skeleton_animation = null;
}
CharAnimationSequence* CharAnimationImporter::importSequence( CharAnimationAction* action, const QString& filename){
	assert( _pending_animation_filenames.isEmpty() );
	_pending_animation_filenames.add( filename );
	loadNextAnimation();
	assert( _pending_animation_filenames.isEmpty() );
	_current_sequence = new CharAnimationSequence();
	QFileInfo fi( filename );
	_current_sequence->_name = fi.baseName();
	_current_sequence->_action = action;
	_animations->_sequences.add( _current_sequence );
	CharAnimationState* prev_state = null;
	QVector3D prev_root_position;
	QVector3D total_translation;
	for( int s = 0; s < _skeleton_animation->stepCount(); s++ ){
		importStep( s );
		CharAnimationState* state = new CharAnimationState( _current_pose, _skeleton_animation->timeAtStep( s ) );
		state->_name = QString::number( s ) + "/" + QString::number( _skeleton_animation->stepCount() -1 ) + "_" + _current_animation_name;
		state->_l_foot_fixed = this->_left_foot.on_ground;
		state->_r_foot_fixed = this->_right_foot.on_ground;
		_current_sequence->_states.add( state );
		_current_sequence->insert( state );
		_animations->_states.add( state );
		if( 0 < s ){
			CharAnimationTransition* transition = new CharAnimationTransition();
			transition->_action = action;
			transition->_to_state = state;
			transition->_duration = ( _skeleton_animation->timeAtStep( s ) - _skeleton_animation->timeAtStep( s -1 ));
			transition->_translation = ( _root_position - prev_root_position ) / _character_importer->_character_type->scale();
			//qDebugV3( "  sub translation", transition->_translation );
			total_translation += transition->_translation;
			transition->_translation.setX( 0 );
			transition->_translation.setZ( 0 );
			prev_state->_out_transitions.add( transition );
			_current_sequence->_transitions.add( transition );
			_animations->_transitions.add( transition );
		}
		prev_root_position = _root_position;
		prev_state = state;
	}
	_animation_skeleton_in_step = null;
	delete _skeleton_animation;
	_skeleton_animation = null;
	//qDebugV3( "translation-total", total_translation );
	qDebug() << _current_sequence->_name << _current_sequence->duration();
	return _current_sequence;
}

void CharAnimationImporter::map_one(CharAnimationSequence* from, int from_index, CharAnimationAction* action, CharAnimationSequence* to, int to_index)
{
	assert( from );
	auto from_state = from->_states.at( from_index );
	auto to_state = to->_states.at( to_index );
	{
		auto trans = from_state->findOutTransition( action );
		if( trans ){
			from_state->_out_transitions.removeOne( trans );
		}
	}
	{
		auto trans = to_state->getOutTransition( action );
		from_state->_out_transitions.add( trans );
	}
	/*
	auto trans = from_state->findOutTransition( action );
	if( trans ){
		trans->_to_state = to_state;
	} else {
		trans = new CharAnimationTransition();
		trans->_action = action;
		trans->_duration = 0.333f;
		trans->_to_state = to_state;
		from_state->_out_transitions.add( trans );
	}
	* /
}

void CharAnimationImporter::map_all_to_start(CharAnimationSequence* from, CharAnimationSequence* to)
{
	foreach( CharAnimationState* from_state, from->_states ){
		from_state->_out_transitions.add( to->_transitions.first() );
	}
}
void CharAnimationImporter::map_last_to_start(CharAnimationSequence* from, CharAnimationSequence* to)
{
	CharAnimationState* from_state = from->_states.last();
	from_state->_out_transitions.add( to->_transitions.first() );
}

bool CharAnimationImporter::isCompleted() const
{
	if( !_pending_animation_filenames.isEmpty() )
		return false;
	if( _skeleton_animation )
		return false;
	return true;
}
void CharAnimationImporter::loadNextAnimation(){
	QString filename = _pending_animation_filenames.takeFirst();
	QFileInfo fi( filename );
	_current_animation_name = fi.baseName();
	RnAssImp assimp( filename );
	assimp._worldTransform.scale( 0.1f );
	assimp._worldTransform.rotate( 180, Vec3::Forward );
	assimp._worldTransform.scale( -1, 1, 1 );
	assert( assimp.animationCount() == 1 );
	RnSkeletonAnimation* animation = assimp.loadAnimation( 0 );
	_skeleton_animation = animation;
	if( _animation_skeleton == null ){
		_animation_skeleton = animation->skeleton()->clone();
	}
}
void CharAnimationImporter::importStep(int step){
	assert( 0 <= step && step < _skeleton_animation->stepCount() );
	_animation_skeleton_in_step = _skeleton_animation->skeletonAtStep( step );
	_lines.clear();
	_animation_skeleton_in_step->createJointLines( _lines );
	_root_position = Vec3::Up * 1000;
	_left_foot.on_ground = false;
	_right_foot.on_ground = false;
	foreach( auto client_imp, _character_importer->_bones ){
		auto parent_imp = client_imp->parent_bone;
		RnSkeletonJoint* client_skin_a_joint = _animation_skeleton_in_step->getJointByName( client_imp->skin_root_joint->name() );
		RnSkeletonJoint* parent_skin_a_joint = ( parent_imp ? _animation_skeleton_in_step->getJointByName( parent_imp->skin_root_joint->name() ) : null );
		QMatrix4x4 client_skin_t = client_imp->skin_root_joint->absoluteMatrix();
		QMatrix4x4 parent_skin_t = ( parent_imp ? parent_imp->skin_root_joint->absoluteMatrix() : QMatrix4x4() );
		//QMatrix4x4 rst = pst.inverted() * cst;
		//_lines.createLine( RnPosOri( pst ).position(), RnPosOri( cst ).position(), QColor( 50, 50, 50 ) );
		//_lines.createCross( RnPosOri( cstg ), 0.3f );
		//_lines.createLine( RnPosOri( pst ).position(), RnPosOri( pst * rst ).position(), QColor( 50, 50, 50 ) );

		QMatrix4x4 client_skin_a = client_skin_a_joint->absoluteMatrix();
		QMatrix4x4 parent_skin_a = ( parent_skin_a_joint ? parent_skin_a_joint->absoluteMatrix() : QMatrix4x4() );
		createDebugLine( parent_skin_a, client_skin_a, QColor( 50, 50, 150 ) );
		//_lines.createLine( RnPosOri( psa ).position(), RnPosOri( csa ).position(), QColor( 50, 50, 50 ) );
		//_lines.createCross( RnPosOri( csag ), 0.3f );

		QMatrix4x4 client_bone_t = client_imp->char_bone->joint_T_posori_in_skin().matrix();
		QMatrix4x4 parent_bone_t = ( parent_imp ? parent_imp->char_bone->joint_T_posori_in_skin().matrix() : QMatrix4x4() );
		QMatrix4x4 parent_bone_to_client_bone = parent_bone_t.inverted() * client_bone_t;
//			QMatrix4x4 client_bone_t_calced_from_parent_bone_t = parent_bone_to_client_bone parent_bone_t;
//			//createDebugLine( parent_bone_t, client_bone_t_calced_from_parent_bone_t, QColor( 255, 255, 255 ) );
//			//_lines.createLine( RnPosOri( parent_bone_t ).position(), RnPosOri( client_bone_t ).position(), QColor( 50, 50, 50 ) );
//			//_lines.createCross( RnPosOri( cbt ), 0.3f );

		QMatrix4x4 client_bone_to_skin = client_bone_t.inverted() * client_skin_t;
//			assert( RnPosOri( client_skin_t ).isNearlySame( RnPosOri( client_bone_t * client_bone_to_skin ) ) );


		QMatrix4x4 client_bone_to_skeleton = client_bone_t.inverted() * client_skin_t;
		QMatrix4x4 client_skin_t_calced = client_bone_t * client_bone_to_skeleton;
		//_lines.createCross( RnPosOri( client_skin_t_calced ), 0.5f );
		assert( RnPosOri( client_skin_t ).isNearlySame( RnPosOri( client_bone_t * client_bone_to_skeleton ) ) );

		QMatrix4x4 client_skin_to_bone = client_skin_t.inverted() * client_bone_t;
		QMatrix4x4 client_bone_t_calced_from_skin = client_skin_t * client_skin_to_bone;
		//_lines.createLine( RnPosOri( parent_bone_t ).position(), RnPosOri( client_bone_t_calced_from_skin ))
		//createDebugLine( parent_bone_t, client_bone_t_calced_from_skin, QColor( 255, 255, 255 ) );
		QMatrix4x4 client_bone_a_calced_from_skin = client_skin_a * client_skin_to_bone;
		//_lines.createLine( RnPosOri( parent_bone_t ).position(), RnPosOri( client_bone_t_calced_from_skin ))
		//createDebugLine( parent_skin_a, client_bone_a_calced_from_skin, QColor( 255, 255, 255 ) );
		QMatrix4x4 parent_skin_to_bone = parent_skin_t.inverted() * parent_bone_t;
		QMatrix4x4 parent_bone_a_calced_from_skin = parent_skin_a * parent_skin_to_bone;
		createDebugLine( parent_bone_a_calced_from_skin, client_bone_a_calced_from_skin, QColor( 55, 155, 55 ) );
		QMatrix4x4 relative_matrix = parent_bone_a_calced_from_skin.inverted() *  client_bone_a_calced_from_skin;
		QMatrix4x4 this_relative_matrix = client_imp->char_bone->_relative_T_matrix.inverted() *  relative_matrix;
		QQuaternion rot = QQuaternion::fromRotationMatrix( this_relative_matrix.normalMatrix() );
		if( client_imp->char_bone->isRoot() ){
			_root_position = client_bone_a_calced_from_skin.map( Vec3::Null );
			QVector3D pose_root_position = _root_position;
			pose_root_position.setY( 0 );
			_current_pose.setRootPosition( pose_root_position / _character_importer->_character_type->scale() );
		}
		_current_pose.setRotation( client_imp->char_bone->index_in_character(), rot );

//			foreach( CharCharacter* test_char, _test_characters ){
//				CharBone* bone = test_char->bones().at( client_imp->char_bone->index_in_character() );
//				bone->setRotation( rot );
//				if( bone->hasParent() ){
//					auto j = test_char->_articulation->getJointForType( bone->_link->type()->parentJoint() );
//					auto pxj = j->pxJoint();
//					PxQuat pxrot(  rot.x(), rot.y(), rot.z(), rot.scalar() );
//					pxj->setTargetOrientation( pxrot );
//					pxj->setDriveType( PxArticulationJointDriveType::eTARGET );
//				}
//				test_char->_articulation->pxArticulation()->wakeUp();
//			}
		if( _left_foot.imp == client_imp )
			calcFoot( _left_foot, client_bone_a_calced_from_skin );
		if( _right_foot.imp == client_imp )
			calcFoot( _right_foot, client_bone_a_calced_from_skin );
//			foreach( CharCharacter* test_char, _test_characters ){
//				if( client_imp->name == "Spine_1" ){
//					test_char->_cp_center->setPositionOrientation( client_bone_a_calced_from_skin );
//				}
//				if( _left_foot.on_ground ){
//					RnPosOri po = test_char->_cp_foot_l->attachement()->positionOrientation();
//					po.setPosition( QVector3D( _left_foot.posori.position().x(), po.position().y(), _left_foot.posori.position().z() ) );
//					po.setPosition( _left_foot.posori.position() );
//					po.setOrientation( _left_foot.posori.orientation() );
//					test_char->_cp_foot_l->setPositionOrientation( po );
//					test_char->_cp_foot_l->setEnabled( true );
//				} else {
//					test_char->_cp_foot_l->setEnabled( false );
//				}
//				if( _right_foot.on_ground ){
//					RnPosOri po = test_char->_cp_foot_r->attachement()->positionOrientation();
//					po.setPosition( QVector3D( _right_foot.posori.position().x(), po.position().y(), _right_foot.posori.position().z() ) );
//					po.setPosition( _right_foot.posori.position() );
//					po.setOrientation( _right_foot.posori.orientation() );
//					test_char->_cp_foot_r->setPositionOrientation( po );
//					test_char->_cp_foot_r->setEnabled( true );
//				} else {
//					test_char->_cp_foot_r->setEnabled( false );
//				}

//			}
	}
}
void CharAnimationImporter::animate(float dt){
	_time_to_next_step -= dt;
	if( 0 < _time_to_next_step )
		return;
	_time_to_next_step = 1.0f / 1115.0f;
	if( _skeleton_animation == null ){
		if( _pending_animation_filenames.isEmpty() )
			_pending_animation_filenames = _all_animation_filenames;
		if( _pending_animation_filenames.isEmpty() )
			return;
		loadNextAnimation();
		_step_in_animation = -1;
		_time_to_next_step = 0.2;
	}
	ASSERT( _skeleton_animation );
	_step_in_animation++;
	if( _step_in_animation < _skeleton_animation->stepCount() ){
		importStep( _step_in_animation );
		foreach( CharCharacter* test_char, _test_characters ){
//			test_char->loadPose( _current_pose );
////			CharBone* bone = test_char->bones().at( client_imp->char_bone->index_in_character() );
////			bone->setRotation( rot );
////			if( bone->hasParent() ){
////				auto j = test_char->_articulation->getJointForType( bone->_link->type()->parentJoint() );
////				auto pxj = j->pxJoint();
////				PxQuat pxrot(  rot.x(), rot.y(), rot.z(), rot.scalar() );
////				pxj->setTargetOrientation( pxrot );
////				pxj->setDriveType( PxArticulationJointDriveType::eTARGET );
////			}
//			test_char->_articulation->pxArticulation()->wakeUp();
		}
	} else {
		_animation_skeleton_in_step = null;
		delete _skeleton_animation;
		_skeleton_animation = null;
	}
}

void CharAnimationImporter::calcFoot(CharAnimationImporter::Foot& foot, const QMatrix4x4& a_skin_matrix){
	QVector3D center = RnPosOri( a_skin_matrix ).position();
	static const float ground_threashold = 0.1f;
	foot.on_ground = false;
	foot.posori = a_skin_matrix;
	foreach( QVector3D local_point, foot.imp->local_points ){
		QVector3D global_point = a_skin_matrix.map( local_point );
		if( global_point.z() < ground_threashold ){
			_lines.createLine( center, global_point, QColor( 255, 255, 0 ) );
			foot.on_ground = true;
		}
	}
}
void CharAnimationImporter::createDebugLine(const QMatrix4x4 parent, const QMatrix4x4 client, const QColor& color){
	RnPosOri parent_posori( parent );
	RnPosOri client_posori( client );
	_lines.createLine( parent_posori.position(), client_posori.position(), color );
}
void CharAnimationImporter::render( RnRenderer& renderer ){
	if( _animation_skeleton_in_step ){
		RnSkeleton* skeleton = _character_importer->_skin->skeleton()->clone();
		_skeleton_animation->mapToSkinSkeleton( _skeleton_animation->timeAtStep( _step_in_animation ), *skeleton );
		skeleton->rootJoint()->recalcRelativeMatrix();
		skeleton->rootJoint()->setAbsoluteMatrix( skeleton->rootJoint()->calcPositionOrientation().translated( _render_offset ).matrix() );
		skeleton->update();
		if( !QApplication::queryKeyboardModifiers().testFlag( Qt::AltModifier ) ){
			qint8 _skinPartVariants[ RnSkin::MaxPartVariants ];
			_character_importer->_skin->render2( renderer, skeleton, 0, _skinPartVariants );
		}
//	skeleton->rootJoint()->setAbsoluteMatrix( skeleton->rootJoint()->calcPositionOrientation().translated( Vec3::Up * 30 ).matrix() );
//	skeleton->update();
//	skeleton->render( renderer, RnPosOri() );
		delete skeleton;
	}
	if( !_lines.isEmpty() ){
		_lines.render( renderer, RnPosOri().translated( _render_offset ) );
	}

}

float QQuaternionDiff( const QQuaternion& f, const QQuaternion& t ){
	float error = 0;
	RnOrientation from_orientation( f );
	RnOrientation to_orientation( t );
	error += ( from_orientation.direction() - to_orientation.direction() ).length();
	error += ( from_orientation.up() - to_orientation.up() ).length();
	error += ( from_orientation.right() - to_orientation.right() ).length();
	return error;
}
float RnPosOriDiff( const RnPosOri& f, const RnPosOri& t ){
	float error = 0;
	error += ( f.position() - t.position() ).length() * 100;
	error += QQuaternionDiff( f.orientation().quaternion(), t.orientation().quaternion() ) * 10;
	return error;
}
//void CharAnimationImporter::map_best_best( QString name, CharAnimationAction* action, CharAnimationSequence* from_sequence, CharAnimationSequence* to_sequence ){
//	float errors[200][200];
//	FloatRange errorRange;
//	for( int x = 0; x < from_sequence->_states.size(); x++ ){
//		CharAnimationState* from_state = from_sequence->_states.at( x );
//		_posed_character->loadPose( from_state->_pose );
//		RnPosOri from_l_foot = _posed_character->_leftFoot.bone->_absolute_matrix;
//		RnPosOri from_r_foot = _posed_character->_rightFoot.bone->_absolute_matrix;
//		RnPosOri from_center = _posed_character->root_bone()->_absolute_matrix;
//		for( int y = 0; y < to_sequence->_states.size(); y++ ){
//			CharAnimationState* to_state = to_sequence->_states.at( y );
//			_posed_character->loadPose( to_state->_pose );
//			RnPosOri to_l_foot = _posed_character->_leftFoot.bone->_absolute_matrix;
//			RnPosOri to_r_foot = _posed_character->_rightFoot.bone->_absolute_matrix;
//			RnPosOri to_center = _posed_character->root_bone()->_absolute_matrix;
//			float error = 0;
//			error += RnPosOriDiff( from_l_foot, to_l_foot );
//			error += RnPosOriDiff( from_r_foot, to_r_foot );
//			error += RnPosOriDiff( from_center, to_center );
//			error += ( from_state->_pose.rootPosition() - to_state->_pose.rootPosition() ).length();
//			for( int b = 0; b < CharBoneCount; b++ ){
//				error += QQuaternionDiff( from_state->_pose.rotation( b ), to_state->_pose.rotation( b ) );
//			}
//			errors[ x ][ y ] = error;
//			if( x == 0 && y == 0 )
//				errorRange.set( error, error );
//			errorRange.adjust( error );
//		}
//	}
//	CharAnimationState* best_from_state = null;
//	CharAnimationState* best_to_state = null;
//	int best_x, best_y;
//	float best_factor = 0;
//	QImage img( from_sequence->_states.size(), to_sequence->_states.size(), QImage::Format_RGB32 );

//	img.fill( QColor( 255, 0, 0 ) );
//	for( int x = 0; x < from_sequence->_states.size(); x++ ){
//		CharAnimationState* from_state = from_sequence->_states.at( x );
//		for( int y = 0; y < to_sequence->_states.size(); y++ ){
//			CharAnimationState* to_state = to_sequence->_states.at( y );


//			float error = errors[ x ][ y ];
//			float factor = errorRange.valuetofactor( error );
//			float r = factor * 0.6f;
//			float g = ( 1.0f - factor ) * 0.6f;
//			float b = 0;
//			if( factor <= 0.0001f ){
//				g = 1;
//				b = 1;
//			}
//			QColor color( (int)( 255.0f * r ), (int)( 255.0f * g ), (int)( 255.0f * b ) );
//			img.setPixelColor( x, y, color );
//			if( y == 0 )
//			if( factor < best_factor || best_from_state == null ){
//				best_from_state = from_state;
//				best_to_state = to_state;
//				best_factor = factor;
//				best_x = x;
//				best_y = y;
//			}
//		}
//	}
//	if( !img.save( "/home/rt/temp/char.imp/" + name + ".png" ) ){
//		assert( false );
//	}
//	assert( best_from_state );
//	assert( best_to_state );
//	//assert( best_factor <= 0.0001f );
//	qDebug() << name << best_factor << best_x << from_sequence->_states.size() << best_y << to_sequence->_states.size() ;
//	CharAnimationTransition* best_transistion = best_to_state->getOutTransition( action );
//	best_from_state->_out_transitions.add( best_transistion );
//	qDebug() << "mapping" << best_from_state->_name;
//	qDebug() << "  of   " << best_to_state->_name;
//	qDebug() << "  to   " << best_transistion->_to_state->_name;
//}
/*
*/



//CharImporter::CharImporter()
//	:_collection( null )
//	,_skin( null )
//	,_skin_skeleton( null )
////	,_characterType( null )
////	,_debugMesh( null )
//	,_root_bone( null )
//	,_character_type( null )
//	,_lines( 1000000 )
//{

//}
//CharImporter::~CharImporter(){
//	delete _skin_skeleton;
//}
//CharCharacterType* CharImporter::create(){
//	assert( _collection );
//	assert( !_skin_name.isEmpty() );
//	assert( _skin == null );
//	_skin = _collection->getResource( _skin_name )->asSkin();
//	_skin_skeleton = _skin->skeleton()->clone();
//	_isMocap = _skin_skeleton->findJointByName( "Hips" );
//	if( _isMocap ){
//		setupJointNamesMocap();
//	} else {
//		setupJointNamesMeanim();
//	}
//	calcJoints();
//	mapSkinJointsToBones();
//	calcBones();
//	createCharacterType();
//	assert( _character_type );
//	assert( _character_type->bones().size() == CharBoneCount );
//	return _character_type;
//}
//void CharImporter::calcJoints(){
//	foreach( Bone* joint, _bones ){
//		foreach( RnSkeletonJoint* skin_joint, _skin_skeleton->joints() ){
//			if( skin_joint->name().toLower() == joint->skin_root_joint_name.toLower() ){
//				// qDebug() << "found" << skin_joint->name().toLower() << joint->skin_root_joint_name.toLower();
//				assert( joint->skin_root_joint == null );
//				joint->skin_root_joint = skin_joint;
//			}
//		}
//		assert( joint->skin_root_joint );
//	}
//	foreach( Bone* joint, _bones ){
//		joint->joint_global_posori.setPosition( joint->skin_root_joint->calcPosition() );
//		joint->joint_global_posori.setOrientation( RnOrientation( Vec3::Up, Vec3::Backward ) );
//	}
//	foreach( Side* side, _sides ){
//		side->upperarm.joint_global_posori.setOrientation( RnOrientation( side->direction, Vec3::Up ) );
//		side->lowerarm.joint_global_posori.setOrientation( RnOrientation( side->direction, Vec3::Up ) );
//		side->hand.joint_global_posori.setOrientation( RnOrientation( side->direction, Vec3::Up ) );
//		side->upperleg.joint_global_posori.setOrientation( RnOrientation( Vec3::Down, Vec3::Forward ) );
//		side->lowerleg.joint_global_posori.setOrientation( RnOrientation( Vec3::Down, Vec3::Forward ) );
//		side->foot.joint_global_posori.setOrientation( RnOrientation( Vec3::Forward, Vec3::Up ) );
//	}
//	if( _isMocap ){
//		foreach( Side* side, _sides ){
////			side->foot.joint_global_posori.translate( Vec3::Down * side->foot.joint_global_posori.position().z() * 0.5f );
//		}
//	}
//	foreach( Bone* joint, _bones ){
//		_lines.createCross( joint->joint_global_posori, 0.3f );
//	}
//}
//void CharImporter::mapSkinJointsToBones(){
//	mapSkinJointToBone( _skin_skeleton->rootJoint(), _root_bone );
//}
//void CharImporter::mapSkinJointToBone(RnSkeletonJoint* skin_joint, CharImporter::Bone* bone){
//	foreach( Bone* b, _bones ){
//		if( b->skin_root_joint == skin_joint ){
//			bone = b;
//			break;
//		}
//	}
//	bone->skin_joints.add( skin_joint );
//	foreach( auto child_joint, skin_joint->childs() ){
//		mapSkinJointToBone( child_joint, bone );
//	}
//}
//void CharImporter::calcBones(){
//	QList<QColor> colors;
//	colors.add( QColor( 255, 0, 0 ) );
//	colors.add( QColor( 0, 255, 0 ) );
//	colors.add( QColor( 0, 0, 255 ) );
//	colors.add( QColor( 255, 255, 0 ) );
//	colors.add( QColor( 255, 0, 255 ) );
//	colors.add( QColor( 0, 255, 255 ) );
//	int c = 0;
//	foreach( Bone* bone, _bones ){
//		bone->color = colors.at( c % colors.size() ); c++;
//		if( bone->parent_bone == null ){
//			assert( bone == _root_bone );
//			_lines.createLine( Vec3::Null + Vec3::Backward, bone->joint_global_posori.position(), QColor( 255, 0, 0 ) );
//		} else {
//			_lines.createLine( bone->joint_global_posori.position(), bone->parent_bone->joint_global_posori.position(), QColor( 255, 255, 255 ) );
//		}
//	}
//}
//void CharImporter::createCharacterType(){
//	assert( _character_type == null );
//	float scale = _spine_0.skin_root_joint->calcPosition().z() / 10.755f;
//	_character_type = new CharCharacterType( _skin, scale );
//	foreach( Bone* bone, _bones ){
//		CharBoneType* parent_char_bone = null;
////		RnPosOri joint_T_posori_in_parent_bone;
//		if( bone->parent_bone == null ){
////			joint_T_posori_in_parent_bone = bone->joint_global_posori;
//		} else {
//			parent_char_bone = bone->parent_bone->char_bone;
//			assert( parent_char_bone );
////			joint_T_posori_in_parent_bone = RnPosOri::map( bone->parent_bone->joint_global_posori.matrix().inverted(), bone->joint_global_posori );
//		}
//		bone->char_bone = new CharBoneType( bone->name, parent_char_bone, bone->joint_global_posori );
//		foreach( auto skin_joint, bone->skin_joints ){
//			if( skin_joint->influencesSkin() ){
//				bone->char_bone->addSkinMatrixIndex( skin_joint->skinMatrixIndex() );
//			}
//		}
//		_character_type->addBone( bone->char_bone );
//	}
//	for( int m = 0; m < _skin->meshes().size(); m++ ){
//		RnSkinMesh* mesh = _skin->meshes().at( m );
//		for( int v = 0; v < mesh->meshbuffer()->vertexBuffer().size(); v++ ){
//			const RnSkinVertex& vertex = mesh->meshbuffer()->vertexBuffer().get( v );
//			bool found = false;
//			foreach( Bone* bone, _bones ){
//				foreach( RnSkeletonJoint* skin_joint, bone->skin_joints ){
//					if( skin_joint->skinMatrixIndex() == vertex.getBone0Index() ){
//						assert( !found );
//						found = true;
//						bone->global_points.add( vertex._position );
//						Vec3 point = bone->char_bone->skinInvMatrix().map( vertex._position );
//						bone->boundingBox.unite( point );
//						bone->local_points.add( point );
//						if( _lines.hasSpaceFor( 10 ) ){
//							_lines.createLine( bone->joint_global_posori.position(), vertex._position, bone->color );
//						}
//					}
//				}
//			}
//			assert( found );
//		}
//	}
//	_character_type->_articulation = new PhArticulationType();
//	_character_type->_articulation->setBoundingBox( _character_type->boundingBox() );
//	foreach( Bone* bone, _bones ){
//		PhArticulationLinkType* parent_link = null;
//		if( bone->parent_bone )
//			parent_link = bone->parent_bone->char_bone->_link;
//		bone->char_bone->_link = _character_type->_articulation->createLink( parent_link );
//		bone->char_bone->_link->setStartPosOri( bone->joint_global_posori );
//		PhShape* shape = PhConvexShape::Convert( bone->local_points );
//		//float mass = 0.1f + bone->boundingBox.size().length();
//		float mass = 5.0f;//
//		bone->char_bone->_link->setShapeAndMass( shape, mass );
//		if( bone->parent_bone ){
//			bone->char_bone->_link->parentJoint()->setStartPosOri( bone->joint_global_posori );
////			bone->char_bone->_link->parentJoint()->setPorOriInParentLink( RnPosOri( bone->char_bone->_relative_T_matrix ) );
////			bone->char_bone->_link->parentJoint()->setPorOriInChildLink( RnPosOri( Vec3::Null, RnOrientation( Vec3::Backward, Vec3::Up ) ) );
//		}
//	}
//	_character_type->initialize();
//}
//void CharImporter::render( RnRenderer& renderer ){
//	RnSkeleton* skeleton = _skin->skeleton()->clone();
//	skeleton->rootJoint()->recalcRelativeMatrix();
//	skeleton->rootJoint()->setAbsoluteMatrix( skeleton->rootJoint()->calcPositionOrientation().translated( _render_offset ).matrix() );
//	skeleton->update();
//	if( !QApplication::queryKeyboardModifiers().testFlag( Qt::AltModifier ) ){
//		qint8 _skinPartVariants[ RnSkin::MaxPartVariants ];
//		//_skin->render2( renderer, skeleton, 0, _skinPartVariants );
//	}
//	skeleton->rootJoint()->setAbsoluteMatrix( skeleton->rootJoint()->calcPositionOrientation().translated( Vec3::Up * 30 ).matrix() );
//	skeleton->update();
//	skeleton->render( renderer, RnPosOri() );
//	delete skeleton;
//	if( !_lines.isEmpty() ){
//		_lines.render( renderer, RnPosOri().translated( _render_offset ) );
//	}

//}


//CharRagdollType* CharImporter::LoadRagdollType( const String& filename, Format format ){
//	CharImporter importer( format );
//	AssImp assimp;
//	assimp.open( filename.asStd() );
//	importer.createRagdoll();
//	AssImpSkeleton* skeleton = assimp.loadSkeleton();
//	importer.setupRagdollFromSkeleton( *skeleton );
//	odelete( skeleton );
//	assert( importer._ragdolltype );
//	return importer._ragdolltype;
//}

CharImporter::CharImporter( Format format ){
	setupBones();
	if( format == MocapFormat ){
		setupJointNamesMocap();
	} else if( format == MecanimFormat ){
		setupJointNamesMecanim2();
	} else {
		assert( false );
	}
}
CharImporter::~CharImporter(){
	_bones.removeAll();
	_sides.removeAll();
}
void CharImporter::setupBones(){
	_l.direction = Vec3::Left;
	_r.direction = Vec3::Right;
	_sides.add( &_l );
	_sides.add( &_r );
	_l.name_prefix = "Left";
	_r.name_prefix = "Right";
	_spine_0.name = "Spine_0";
	_spine_0.absoluteTposori.set( Vec3( 0, 0, 1 ), Vec3::Forward, Vec3::Up );
	_spine_1.name = "Spine_1";
	_spine_1.absoluteTposori.set( Vec3( 0, 0, 1.3f ), Vec3::Forward, Vec3::Up );
	_neck.name = "Neck";
	_neck.absoluteTposori.set( Vec3( 0, 0, 1.6f ), Vec3::Forward, Vec3::Up );
	_head.name = "Head";
	_head.absoluteTposori.set( Vec3( 0, 0, 1.8f ), Vec3::Forward, Vec3::Up );
	_bones.add( &_spine_0 );
	_bones.add( &_spine_1 );
	//_bones.add( &_neck );
	_bones.add( &_head );
	for( Side* side : _sides ){
		side->upperarm.name = side->name_prefix + "UpperArm";
		side->lowerarm.name = side->name_prefix + "LowerArm";
		side->hand.name = side->name_prefix + "Hand";
		side->upperleg.name = side->name_prefix + "UpperLeg";
		side->lowerleg.name = side->name_prefix + "LowerLeg";
		side->foot.name = side->name_prefix + "Foot";
		_bones.add( &side->upperarm );
		_bones.add( &side->lowerarm );
		_bones.add( &side->hand );
		_bones.add( &side->upperleg );
		_bones.add( &side->lowerleg );
		_bones.add( &side->foot );
		side->upperarm.parent_bone = &_spine_1;
		side->lowerarm.parent_bone = &side->upperarm;
		side->hand.parent_bone = &side->lowerarm;
		side->upperleg.parent_bone = &_spine_0;
		side->lowerleg.parent_bone = &side->upperleg;
		side->foot.parent_bone = &side->lowerleg;

		side->upperarm.absoluteTposori.set( side->direction * 0.3f + Vec3( 0, 0, 1.6f ), side->direction, Vec3::Up );
		side->lowerarm.absoluteTposori.set( side->direction * 1.3f + Vec3( 0, 0, 1.6f ), side->direction, Vec3::Up );
		side->hand.absoluteTposori.set( side->direction * 1.5f + Vec3( 0, 0, 1.6f ), Vec3::Down, side->direction );
		side->upperleg.absoluteTposori.set( side->direction * 0.3f + Vec3( 0, 0, 0.6f ), Vec3::Forward, Vec3::Up );
		side->lowerleg.absoluteTposori.set( side->direction * 0.3f + Vec3( 0, 0, 0.3f ), Vec3::Forward, Vec3::Up );
		side->foot.absoluteTposori.set( side->direction * 0.3f + Vec3( 0, 0, 0.1f ), Vec3::Forward, Vec3::Up );

	}
	_root_bone = &_spine_0;
	_spine_1.parent_bone = &_spine_0;
	_neck.parent_bone = &_spine_1;
	_head.parent_bone = &_neck;
	_head.parent_bone = &_spine_1;
}
void CharImporter::setupJointNamesMocap(){
	_head.skin_root_joint_name = "Head";
	_neck.skin_root_joint_name = "Neck";
	_spine_1.skin_root_joint_name = "Spine2";
	_spine_0.skin_root_joint_name = "Spine";
_spine_0.skin_root_joint_name = "Hips";
	_l.skin_joint_ext_name = "Left";
	_r.skin_joint_ext_name = "Right";
	for( Side* side : _sides ){
		side->upperarm.skin_root_joint_name = side->skin_joint_ext_name + "Arm";
		side->lowerarm.skin_root_joint_name = side->skin_joint_ext_name + "ForeArm";
		side->hand.skin_root_joint_name = side->skin_joint_ext_name + "Hand";
		side->upperleg.skin_root_joint_name = side->skin_joint_ext_name + "UpLeg";
		side->lowerleg.skin_root_joint_name = side->skin_joint_ext_name + "Leg";
		side->foot.skin_root_joint_name = side->skin_joint_ext_name + "Foot";
	}
}
void CharImporter::setupJointNamesMecanim1(){
	_head.skin_root_joint_name = "Head";
	_neck.skin_root_joint_name = "Neck_01";
	_spine_1.skin_root_joint_name = "Spine_02";
	_spine_0.skin_root_joint_name = "Spine_01";
	_l.skin_joint_ext_name = "_L";
	_r.skin_joint_ext_name = "_R";
	for( Side* side : _sides ){
		side->upperarm.skin_root_joint_name = String( "UpperArm" )+ side->skin_joint_ext_name;
		side->lowerarm.skin_root_joint_name = String( "LowerArm" ) + side->skin_joint_ext_name;
		side->hand.skin_root_joint_name = String( "Hand" ) + side->skin_joint_ext_name;
		side->upperleg.skin_root_joint_name = String( "Thigh" ) + side->skin_joint_ext_name;
		side->lowerleg.skin_root_joint_name = String( "Calf" ) + side->skin_joint_ext_name;
		side->foot.skin_root_joint_name = String( "Foot" ) + side->skin_joint_ext_name;
	}
}
void CharImporter::setupJointNamesMecanim2(){
	_head.skin_root_joint_name = "Head";
	_neck.skin_root_joint_name = "Neck_01";
	_spine_1.skin_root_joint_name = "Spine_02";
	_spine_0.skin_root_joint_name = "Pelvis";
	_l.skin_joint_ext_name = "_L";
	_r.skin_joint_ext_name = "_R";
	for( Side* side : _sides ){
		side->upperarm.skin_root_joint_name = String( "UpperArm" )+ side->skin_joint_ext_name;
		side->lowerarm.skin_root_joint_name = String( "LowerArm" ) + side->skin_joint_ext_name;
		side->hand.skin_root_joint_name = String( "Hand" ) + side->skin_joint_ext_name;
		side->upperleg.skin_root_joint_name = String( "Thigh" ) + side->skin_joint_ext_name;
		side->lowerleg.skin_root_joint_name = String( "Calf" ) + side->skin_joint_ext_name;
		side->foot.skin_root_joint_name = String( "Foot" ) + side->skin_joint_ext_name;
	}
}
void CharImporter::createRagdoll(){
	assert( _ragdolltype == null );
	_ragdolltype = new CharRagdollType();
	for( Bone* bone : _bones ){
		assert( bone->joint == null );
		CharJointType* parent_joint = null;
		if( bone->parent_bone ){
			parent_joint = asserted( bone->parent_bone->joint );
		}
		bone->joint = _ragdolltype->createJoint( parent_joint );
		bone->joint->_name = bone->name;
	}
}
void CharImporter::setupRagdollFromSkeleton( const Skeleton& skeleton ){
	for( Bone* bone : _bones ){
		SkeletonJoint* skeletonjoint = skeleton.getJointByName( bone->skin_root_joint_name );
		bone->skin_absoluteTposori = PosOri( skeletonjoint->absolutematrix() ).matrix();
		if( bone->parent_bone == null ){
			bone->skin_relativeTposori = bone->skin_absoluteTposori;
		} else {
			bone->skin_relativeTposori = bone->parent_bone->skin_absoluteTposori.inverted() * bone->skin_absoluteTposori;
		}
		bone->absoluteTposori.setPosition( PosOri( bone->skin_absoluteTposori ).position() );
//bone->absoluteTposori = PosOri( bone->skin_absoluteTposori );
	}
this->_l.foot.absoluteTposori.setPosition( this->_l.foot.absoluteTposori.position().zed( 0.05f ) );
this->_r.foot.absoluteTposori.setPosition( this->_r.foot.absoluteTposori.position().zed( 0.05f ) );
	for( Bone* bone : _bones ){
		assert( bone->joint );
		if( bone->parent_bone == null ){
			bone->joint->_relativeTposori = bone->absoluteTposori;
		} else {
			bone->joint->_relativeTposori = PosOri( bone->parent_bone->absoluteTposori.matrix().inverted() * bone->absoluteTposori.matrix() );
		}
//bone->joint->_relativeTposori = bone->absoluteTposori;
//logDebug( "  T ", bone->name, bone->joint->relativeTposori().position() );
	}

}
void CharImporter::loadSkin( const Skeleton& skeleton, AssImp& assimp, uint meshindex ){
	_testmesh = new MeshPNT( "CharImporter.loadSkin" );
	assert( _ragdolltype );
	assert( _ragdolltype->_skin == null );
	_ragdolltype->_skin = SkinImporter::Import( assimp, 0 );
	for( SkinBoneType* skinbone : _ragdolltype->_skin->bones() ){
		SkeletonJoint* skeletonjoint = skeleton.getJointByName( skinbone->jointname );
		Bone* bone = getBoneForSkeletonJoint( skeletonjoint );
		assert( bone->joint );
		CharRagdollType::JointToSkin* joint2skin = new CharRagdollType::JointToSkin();
		joint2skin->joint = bone->joint;
		joint2skin->relativematrix = bone->absoluteTposori.matrix().inverted() * skeletonjoint->absolutematrix();
		_ragdolltype->_joint2skins.add( joint2skin );
	}

	Bone* skeletonjoint_to_bone[ skeleton.joints().size() ];
	for( SkeletonJoint* skeletonjoint : skeleton.joints() ){
		skeletonjoint_to_bone[ skeletonjoint->index() ] = findBoneForSkeletonJoint( skeletonjoint );
	}
	const Vec3 co = Vec3::Up * 0.1f;
	for( Bone* bone : _bones ){
		PosOri po( bone->absoluteTposori.translated( co ) );
		logDebug( "uuuuuuuuu", po.up() );
		MeshBuilder::CreatePosOri( *_testmesh, bone->absoluteTposori.translated( co ), 0.02f, 0.002f, 6, true, VertexPNT() );
		if( bone->parent_bone ){
			MeshBuilder::CreateLine( *_testmesh, bone->absoluteTposori.translated( co ).position(),
									 bone->parent_bone->absoluteTposori.translated( co ).position(),
									 0.001f, 6, true, VertexPNT() );

		}
		SkeletonJoint* skeletonjoint = skeleton.getJointByName( bone->skin_root_joint_name );
		MeshBuilder::CreateLine( *_testmesh, PosOri( skeletonjoint->absolutematrix() ).position(),
								 bone->absoluteTposori.translated( co ).position(), 0.001f, 6, true, VertexPNT() ) ;

	}
	for( SkeletonJoint* skeletonjoint : skeleton.joints() ){
		MeshBuilder::CreatePosOri( *_testmesh, PosOri( skeletonjoint->absolutematrix() ), 0.02f, 0.002f, 6, true, VertexPNT() );
		if( skeletonjoint->hasParent() ){
			MeshBuilder::CreateLine( *_testmesh, PosOri( skeletonjoint->absolutematrix() ).position(),
									 PosOri( skeletonjoint->parent()->absolutematrix() ).position(), 0.001f, 6, true, VertexPNT() ) ;
		}
//		Bone* bone = skeletonjoint_to_bone[ skeletonjoint->index() ];
		Bone* bone = findBoneForSkeletonJoint( skeletonjoint );
		if( bone == null ){
			logDebug( "sk ", skeletonjoint->name(), "NULL" );
//			MeshBuilder::CreateLine( *_testmesh, PosOri( skeletonjoint->absolutematrix() ).position(), Vec3::Null, 0.002f, 6, true, VertexPNT() ) ;
		} else {
			logDebug( "sk ", skeletonjoint->name(), bone->name );
			MeshBuilder::CreateLine( *_testmesh, PosOri( skeletonjoint->absolutematrix() ).position(),
									 bone->absoluteTposori.position() + co, 0.002f, 6, true, VertexPNT() ) ;
		}
	}
	aiMesh* aimesh = assimp.aimesh( meshindex );
	MeshPNT* fullmesh = assimp.loadMeshPNT( meshindex );
	float vertexweights[ fullmesh->vertexCount() ];
	for( Bone* bone : _bones ){
		for( int v = 0; v < fullmesh->vertexCount(); v++ ){
			vertexweights[ v ] = 0;
		}
		for( unsigned int b = 0; b < aimesh->mNumBones; b++ ){
			aiBone* aibone = aimesh->mBones[b];
			String aiboneName( aibone->mName.C_Str() );
			if( aibone->mNode == null ){
				assert( false );
			} else {
				aiboneName = aibone->mNode->mName.C_Str();
			}
			SkeletonJoint* aijoint = skeleton.getJointByName( aiboneName );
			if( skeletonjoint_to_bone[ aijoint->index() ] == bone ){
				for( uint w = 0; w < aibone->mNumWeights; w++ ){
					aiVertexWeight* weights = &aibone->mWeights[ w ];
					ASSERT( weights->mVertexId < fullmesh->vertexCount() );
					vertexweights[ weights->mVertexId ] += weights->mWeight;
				}
			}
		}
		MeshPNT* partmesh = MeshBuilder::FilteredByVertex<VertexPNT>( *fullmesh, [&]( int v, const VertexPNT& vertex ){
			return 0.1f < vertexweights[ v ];
		}, false );
		if( partmesh->indexCount() < 1 ){
			//assert( false );
			logError( "bone is empty ", bone->joint->name() );
			odelete( partmesh );
		} else {
			partmesh->transform( bone->absoluteTposori.matrix().inverted() );
			assert( bone->joint->_mesh == null );
			bone->joint->_mesh = partmesh;
		}
	}
	odelete( fullmesh );
}
void CharImporter::mapSkeletonToRagdoll( const Skeleton& skeleton, CharRagdoll& ragdoll ){
	skeleton.trace();
	for( Bone* bone : _bones ){
		CharJoint* joint = ragdoll.getJointByName( bone->name );
		SkeletonJoint* skeletonjoint = skeleton.getJointByName( bone->skin_root_joint_name );
		logDebug( "  ", skeletonjoint->name(), PosOri( skeletonjoint->absolutematrix() ).position() );
		Mat4 skeletonrelative = skeleton.calcRelativeMatrix( bone->skin_root_joint_name, ( bone->parent_bone ? bone->parent_bone->skin_root_joint_name : String() ) );
//		logDebug( "     x ", skeletonrelative.map( Vec3::Null ) );
////		if( bone->parent_bone == null ){
////			skeletonrelative = skeletonjoint->absolutematrix();
////		} else {
////			AssImpSkeletonJoint* parentskeletonjoint = skeleton.getJointByName( bone->parent_bone->skin_root_joint_name );
////			skeletonrelative = parentskeletonjoint->absolutematrix().inverted() * skeletonjoint->absolutematrix();
////		}
//		logDebug( "     ", PosOri( skeletonrelative ).position() );
		skeletonrelative = PosOri( skeletonrelative ).matrix();
//		logDebug( "     b ", bone->skin_relativeTposori.map( Vec3::Null ) ); //
//		logDebug( "     s ", skeletonrelative.map( Vec3::Null ) );
//		Mat4 u = bone->skin_relativeTposori.inverted() * skeletonrelative;
//		logDebug( "     u ", u.map( Vec3::Null ) );
//////		static float f = 0;
//////		f += 0.1f;
//////		joint->setRotation( Mat4::RotationAtAxis( f, Vec3( 1, 1, 1 ).normalized() ).getRotation() );
////		logDebug( "uuuuu", u.map( Vec3::Null ) );
//		glm::quat r = u.getRotation();
//		logDebug( "     r ", r.w, r.x, r.y, r.z );
		//joint->setRotation( u.getRotation() );
//		joint->setAbsoluteMatrix( skeletonjoint->absolutematrix() );
//		joint->setAbsoluteMatrix( bone->absoluteTposori.matrix() * u );
		//skeletonrelative = bone->joint->relativeTposori().matrix(); //
		//skeletonrelative = bone->joint->relativeTposori().matrix() * bone->skin_relativeTposori.inverted() * skeletonrelative;
		// bone->joint->relativeTposori().matrix()  * ( skeletonrelative * bone->skin_relativeTposori.inverted() );
		//joint->setRelativeMatrix( skeletonrelative );
//		joint->setRelativeMatrix( bone->relativeTposori );
		PosOri skin_abs( skeletonjoint->absolutematrix() );
		joint->setAbsoluteMatrix( skin_abs.matrix() );
		PosOri j_abs( skin_abs.matrix() * bone->skin_absoluteTposori.inverted() * bone->absoluteTposori.matrix() ); //   * bone->skin_absoluteTposori.inverted() * skin_abs.matrix() );
		joint->setAbsoluteMatrix( j_abs.matrix() );
		Mat4 joint_relativematrix;
		if( joint->hasParent() == false ){
			joint_relativematrix = joint->absolutematrix();
		} else {
			joint_relativematrix = joint->parent()->absolutematrix().inverted() * joint->absolutematrix();
		}
//		joint->setRelativeMatrix( joint_relativematrix );
		Mat4 rotation_matrix = joint->type()->relativeTposori().matrix().inverted() * joint_relativematrix;
//		joint->setRelativeMatrix( joint->type()->relativeTposori().matrix() );
		joint->setRotation( rotation_matrix.getRotation() );
	}
	//assert( false );
}
CharImporter::Bone*CharImporter::findBoneForSkeletonJoint( SkeletonJoint* aijoint ) const {
	ASSERT( aijoint );
	for( Bone* bone : _bones ){
		if( bone->skin_root_joint_name.equalsIgnoreCase( aijoint->name() ) )
			return bone;
	}
	if( aijoint->hasParent() )
		return findBoneForSkeletonJoint( aijoint->parent() );
	return null;
}
CharImporter::Bone* CharImporter::getBoneForSkeletonJoint( SkeletonJoint* aijoint ) const {
	CharImporter::Bone* bone = findBoneForSkeletonJoint( aijoint );
	assert( bone );
	return bone;
}
CharAnimation* CharImporter::loadAnimation( const AssImpAnimation& skinanimation ){
	CharAnimation* charanimation = new CharAnimation();
	CharRagdoll ragdoll( ragdolltype() );
	for( AssImpAnimationFrame* skinanimframe : skinanimation.frames() ){
		Skeleton* skinskeleton = skinanimframe->skeleton();
		mapSkeletonToRagdoll( *skinskeleton, ragdoll );
		CharAnimationFrame* charframe = new CharAnimationFrame();
		charframe->_duration = skinanimframe->duration();
		ragdoll.savePose( charframe->_pose );
		charframe->_pose._translation =
				skinskeleton->getJointByName( this->_root_bone->skin_root_joint_name )->absolutematrix().map( Vec3::Null )
				- _root_bone->joint->relativeTposori().position();
		//.zed( 0 );
//				skinskeleton->root()->absolutematrix().map( Vec3::Null );
		//	Vec3::Forward * skinanimframe->starttime();
		charanimation->_frames.add( charframe );
	}
	charanimation->recalc();
	return charanimation;

}
